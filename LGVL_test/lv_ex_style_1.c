#include "../../lv_examples.h"
#include "string.h"
#include "time.h"

lv_obj_t* btnmatrix;
lv_obj_t* textArea;
extern const lv_img_dsc_t test;

static const char* btn_map[] = {"1","2","3","\n",
                                "4","5","6","\n",
                                "7","8","9","\n",
                                LV_SYMBOL_OK,"0",LV_SYMBOL_BACKSPACE,""
                                };
lv_obj_t* msg;
static void home_cb(lv_obj_t *cont,lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        static const char* btn[] = { "Cancel" ,""};
        lv_textarea_set_text(textArea, "");
        lv_obj_t* obj = lv_obj_create(lv_scr_act(),NULL);
        lv_obj_reset_style_list(obj, LV_OBJ_PART_MAIN);
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj,LV_HOR_RES, LV_VER_RES);
        msg= lv_msgbox_create(obj,NULL);
        lv_msgbox_set_text(msg,"Hello");
        lv_obj_set_width(msg,200);
        lv_obj_align(msg, NULL, LV_ALIGN_IN_TOP_LEFT, 36, 100);
        lv_obj_t *text = lv_textarea_create(msg,NULL);
        lv_textarea_set_one_line(text,true);
        lv_obj_align(text, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_msgbox_add_btns(msg,btn);
        lv_obj_set_width(text, 180);
    }
}
static void btn_cb(lv_obj_t *btn,lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        const char* txt = lv_btnmatrix_get_active_btn_text(btn);
        if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
        {
            lv_textarea_del_char(textArea);
        }
        else if(strcmp(txt, LV_SYMBOL_OK) == 0)
        {
            lv_textarea_set_text(textArea, "");
        }
        else
        {
            lv_textarea_add_text(textArea, txt);
        }  
    }
}
LV_IMG_DECLARE(test);
/**
 * Using the background style properties
 */
void lv_ex_style_1(void)
{
    static lv_point_t vaildpoint[] = { {0,0},{0,1} };

    lv_obj_t* tileview = lv_tileview_create(lv_scr_act(), NULL);
    lv_tileview_set_valid_positions(tileview, vaildpoint, 2);
    lv_tileview_set_edge_flash(tileview, true);

    lv_obj_t* tile1 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile1, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(tile1,0,0);
    lv_tileview_add_element(tileview, tile1);

    lv_obj_t* tile2 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile2, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(tile2,0, LV_VER_RES);
    lv_tileview_add_element(tileview, tile2);

    lv_obj_t* label0 = lv_label_create(tile1, NULL);
    lv_obj_set_size(label0, LV_HOR_RES, 32);
    lv_obj_align(label0, NULL, LV_ALIGN_IN_TOP_MID, -32, 4);
    lv_label_set_align(label0,LV_LABEL_ALIGN_CENTER);
    
    time_t t;
    t = time(NULL);
    struct tm* tclock;
    tclock = localtime(&t);
    lv_label_set_text_fmt(label0, "%4d-%02d-%02d\n%02d:%02d:%02d",(tclock->tm_year+1900),
                                                                    tclock->tm_mon,
                                                                    tclock->tm_mday,
                                                                    tclock->tm_hour,
                                                                    tclock->tm_min,
                                                                    tclock->tm_sec);

    lv_obj_t* cont = lv_cont_create(tile1,NULL);
    lv_obj_set_auto_realign(cont,true);
    lv_cont_set_layout(cont,LV_LAYOUT_COLUMN_MID);
    lv_obj_set_size(cont,272,392);

    lv_obj_t* image = lv_img_create(cont, NULL);
    lv_img_set_src(image,&test);
    
    //lv_obj_t* cont1 = lv_cont_create(tile1,NULL);
    //lv_obj_align(cont1,NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 8);
    //lv_cont_set_layout(cont1, LV_LAYOUT_COLUMN_MID);
    //lv_cont_set_fit(cont1,LV_FIT_TIGHT);
    //lv_obj_set_auto_realign(cont1, true);
   

    lv_obj_t *label = lv_label_create(tile1,NULL);
    //lv_obj_set_size(label, LV_HOR_RES, LV_VER_RES / 100 * 6);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(label,"wangxiahua");
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -8);

    lv_obj_t* cont2 = lv_cont_create(tile2,NULL);
    lv_obj_set_auto_realign(cont2,true);
    lv_obj_align(cont2, NULL, LV_ALIGN_IN_TOP_RIGHT, -8, -8);
    lv_cont_set_fit(cont2, LV_FIT_TIGHT);
    lv_cont_set_layout(cont2,LV_LAYOUT_COLUMN_MID);
    lv_obj_set_event_cb(cont2, home_cb);

    lv_obj_t* label2 = lv_label_create(cont2,NULL);
    lv_label_set_align(label2, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(label2, LV_SYMBOL_HOME);
   
    textArea = lv_textarea_create(tile2, NULL);
    lv_textarea_set_one_line(textArea,true);
    lv_obj_align(textArea, NULL, LV_ALIGN_IN_TOP_LEFT, LV_HOR_RES / 10, LV_VER_RES / 10 * 1.5);
    lv_obj_set_size(textArea, LV_HOR_RES/5*4, LV_VER_RES / 10);
    lv_textarea_set_text(textArea, "");
    lv_textarea_set_pwd_mode(textArea,true);
    lv_textarea_set_pwd_show_time(textArea,800);
    lv_textarea_set_max_length(textArea, 16);
    lv_textarea_set_text_align(textArea,LV_LABEL_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(textArea,"qingshurumima");

    btnmatrix = lv_btnmatrix_create(tile2,NULL);
    lv_btnmatrix_set_map(btnmatrix, btn_map);
    lv_obj_set_size(btnmatrix, LV_HOR_RES/10*9, LV_VER_RES/8*5);
    lv_obj_align(btnmatrix, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
    lv_obj_set_event_cb(btnmatrix,btn_cb);

}

