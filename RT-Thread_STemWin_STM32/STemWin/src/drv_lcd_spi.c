#include <board.h>
//#include "drv_lcd.h"
#include "string.h"

//#define DRV_DEBUG
//#define LOG_TAG "drv.lcd"
//#include <drv_log.h>

extern _lcd_dev lcddev;
//SRAM_HandleTypeDef hsram1;

//#define LCD_BL GET_PIN(B, 15)
//#define LCD_BASE ((uint32_t)(0x6C000000 | 0x0000007E))
//#define LCD ((LCD_CONTROLLER_TypeDef *)LCD_BASE)

#define LCD_DEVICE(dev) (struct drv_lcd_device *)(dev)

struct drv_lcd_device
{
    struct rt_device parent;

    struct rt_device_graphic_info lcd_info;
};

static struct drv_lcd_device _lcd;

static void LCD_Fast_DrawPoint(const char *pixel, int x, int y)
{
	LCD_DrawPoint(x,y,*pixel);
}
void LCD_HLine(const char *pixel, int x1, int x2, int y)
{
	LCD_Fill(x1,y,x2,y,*pixel);
}
static rt_err_t drv_lcd_init(struct rt_device *device)
{
	LCD_Init();
	return RT_EOK;
}

struct rt_device_graphic_ops spi_lcd_ops =
    {
        LCD_Fast_DrawPoint,
        RT_NULL,
        LCD_HLine,
        RT_NULL,
        RT_NULL,
};

static rt_err_t drv_lcd_control(struct rt_device *device, int cmd, void *args)
{
    struct drv_lcd_device *lcd = LCD_DEVICE(device);
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_GET_INFO:
    {
        struct rt_device_graphic_info *info = (struct rt_device_graphic_info *)args;

        RT_ASSERT(info != RT_NULL);
        
        //this needs to be replaced by the customer
        info->pixel_format  = lcd->lcd_info.pixel_format;
        info->bits_per_pixel = lcd->lcd_info.bits_per_pixel;
        info->width = lcddev.width;
        info->height = lcddev.height;
    }
    break;
    }

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops lcd_ops =
    {
        drv_lcd_init,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        drv_lcd_control};
#endif

int drv_lcd_hw_init(void)
{
    rt_err_t result = RT_EOK;
    struct rt_device *device = &_lcd.parent;
    /* memset _lcd to zero */
    memset(&_lcd, 0x00, sizeof(_lcd));

    _lcd.lcd_info.bits_per_pixel = 16;
    _lcd.lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB565;

    device->type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    device->ops = &lcd_ops;
#else
    device->init = drv_lcd_init;
    device->control = drv_lcd_control;
#endif
    device->user_data = &spi_lcd_ops;
    /* register lcd device */
    rt_device_register(device, "lcd", RT_DEVICE_FLAG_WRONLY | RT_DEVICE_FLAG_STANDALONE);

    return result;
}
INIT_DEVICE_EXPORT(drv_lcd_hw_init);
