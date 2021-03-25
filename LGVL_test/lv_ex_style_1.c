#include "../../lv_examples.h"
#include "string.h"
#include "time.h"

lv_obj_t* textArea;
lv_obj_t* cont2;
lv_obj_t* obj0;
lv_obj_t* btnm1;
lv_obj_t* btnm2;
static lv_style_t style;

extern const lv_img_dsc_t test;

static void btnmartix_create(lv_obj_t* btnmatrix, lv_obj_t* obj, lv_event_cb_t event_cb);
static void msg_btn_cb(lv_obj_t* msg, lv_event_t event);
static void msg_edit_cb(lv_obj_t* msg, lv_event_t event);
static void home_cb(lv_obj_t* cont, lv_event_t event);
static void btn_cb(lv_obj_t* btn, lv_event_t event);

static const char* btn_map[] = {"1","2","3","\n",
                                "4","5","6","\n",
                                "7","8","9","\n",
                                LV_SYMBOL_OK,"0",LV_SYMBOL_BACKSPACE,""
                                };
lv_obj_t* msg;
static void btnmartix_create(lv_obj_t* btnmatrix,lv_obj_t *obj,lv_event_cb_t event_cb)
{
    btnmatrix = lv_btnmatrix_create(obj, NULL);
    lv_btnmatrix_set_map(btnmatrix, btn_map);
    lv_obj_set_size(btnmatrix, LV_HOR_RES / 10 * 9, LV_VER_RES / 8 * 5);
    lv_obj_align(btnmatrix, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
    lv_obj_set_event_cb(btnmatrix, event_cb);
}
static void mag_cancelbtn_cb(lv_obj_t* mgsb, lv_event_t event)
{
    if ((event == LV_EVENT_DELETE)&&(mgsb==msg))
    {
        lv_obj_del_async(obj0);
        msg = NULL;
    }
    else if (event ==LV_EVENT_VALUE_CHANGED)
    {
        lv_msgbox_start_auto_close(msg,0);

    }
       
}
static int pwd_check(const char *pwd,int mode)
{
    if (mode)
    {
        if (strcmp(pwd, "123457") == 0)
        {
            return 1;
        }
    }
    else
    {
        if (strcmp(pwd, "123456") == 0)
        {
            return 1;
        }
    }
    return 0;
}
static void msg_btn_cb(lv_obj_t *msgb,lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t* child_textarea = lv_obj_get_child(msg,NULL);
        child_textarea = lv_obj_get_child(msg, child_textarea);
        const char* txt = lv_btnmatrix_get_active_btn_text(msgb);
        if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
        {
            lv_textarea_del_char(child_textarea);
        }
        else if (strcmp(txt, LV_SYMBOL_OK) == 0)
        {
            if (pwd_check(lv_textarea_get_text(child_textarea),1))
                lv_textarea_set_text(child_textarea, "");
        }
        else
        {
            lv_textarea_add_text(child_textarea, txt);
        }
    }
}
static void msg_edit_cb(lv_obj_t *msge,lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_obj_t* obj = lv_obj_get_screen(msge);
        lv_obj_align(msg,obj,LV_ALIGN_IN_TOP_MID,0,5);
        btnmartix_create(btnm2,obj0, msg_btn_cb);
    }
}
static void opa_anim(void* bg, lv_anim_value_t v)
{
    lv_obj_set_style_local_bg_opa(bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, v);
}
static void home_cb(lv_obj_t *cont,lv_event_t event)
{
    if ((event == LV_EVENT_CLICKED)&&(cont==cont2))
    {
        static const char* btn[] = { "Cancel" ,""};

        lv_style_init(&style);
        lv_style_set_bg_color(&style,LV_STATE_DEFAULT,LV_COLOR_BLACK);
        lv_textarea_set_text(textArea, "");

        obj0 = lv_obj_create(lv_obj_get_parent(cont),NULL);
        lv_obj_reset_style_list(obj0, LV_OBJ_PART_MAIN);
        lv_obj_add_style(obj0,LV_OBJ_PART_MAIN,&style);
        lv_obj_set_pos(obj0, 0, 0);
        lv_obj_set_size(obj0,LV_HOR_RES, LV_VER_RES);

        msg= lv_msgbox_create(obj0,NULL);
        lv_msgbox_set_text(msg,"Hello");
        lv_obj_set_width(msg,200);
        lv_obj_align(msg, NULL, LV_ALIGN_IN_TOP_LEFT, 36, 80);

        lv_obj_t *text = lv_textarea_create(msg,NULL);
        lv_textarea_set_one_line(text,true);
        lv_obj_align(text, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_event_cb(text,msg_edit_cb);
        lv_textarea_set_text(text,"");
        lv_textarea_set_text_align(text,LV_LABEL_ALIGN_CENTER);
        lv_textarea_set_placeholder_text(text,"sftsrf");
        lv_textarea_set_pwd_mode(text,true);
        lv_textarea_set_pwd_show_time(text,800);
        lv_textarea_set_max_length(text, 16);
        lv_msgbox_add_btns(msg,btn);
        lv_obj_set_width(text, 180);
        lv_obj_set_event_cb(msg, mag_cancelbtn_cb);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj0);
        lv_anim_set_time(&a, 500);
        lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_50);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)opa_anim);
        lv_anim_start(&a);
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
            if (pwd_check(lv_textarea_get_text(textArea), 0))
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
    

    lv_obj_t *label = lv_label_create(tile1,NULL);
    //lv_obj_set_size(label, LV_HOR_RES, LV_VER_RES / 100 * 6);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(label,"wangxiahua");
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -8);

    cont2 = lv_cont_create(tile2,NULL);
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

    btnmartix_create(btnm1,tile2,btn_cb);

}

