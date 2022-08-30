#include "gui.h"
#include "usart.h"

LV_IMG_DECLARE(temp_img)
LV_FONT_DECLARE(Font)

static lv_style_t style_cn;

u8 Belt=0;

lv_obj_t *src;
/*系统初始GUI参数*/
lv_obj_t *btn1;
lv_obj_t *label1;
lv_obj_t *label2;
lv_obj_t *label3;
lv_obj_t *image1;

/*系工程面板GUI参数*/
lv_obj_t *works_btn1;
lv_obj_t *works_btn2;
lv_obj_t *works_btn3;


lv_obj_t *led1;
lv_obj_t *led2;
/*gauge面板GUI参数*/
lv_obj_t *gauge_btn1;
lv_obj_t *gauge1;
lv_obj_t *gauge2;
lv_obj_t *gauge3;

void Works_gui(void);
void gauge_gui(void);

void works_btn1_cd(lv_obj_t * obj, lv_event_t event)
{
	if(obj==works_btn1)
	{
		switch(event)
		{
			case LV_EVENT_PRESSED:
			{	Belt=~Belt;
				if(Belt)
				{
					TIM_SetCompare4(TIM1,150);
					lv_led_on(led2);
				}
				else
				{
					TIM_SetCompare4(TIM1,0);
					lv_led_off(led2);
				}
			}
		}
	}
	else
		if(obj==works_btn2)
		{
			switch(event)
			{
				case LV_EVENT_PRESSED:
				{
					lv_obj_clean(src);
					user_gui_init();
					//NVID_DISABLE();
				}break;
			}
		}
		else
			if(obj==works_btn3)
			{
				switch(event)
				{
					case LV_EVENT_PRESSED:
					{
						lv_obj_clean(src);
						gauge_gui();
					}break;
				}
			}
}
void btn1_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch(event)
	{
		case LV_EVENT_PRESSED:
		{
			lv_obj_clean(src);
			Works_gui();
			NVID_Init();
		}
	}
}

void gauge_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch(event)
	{
		case LV_EVENT_PRESSED:
		{
			lv_obj_clean(src);
			Works_gui();
		}
	}	
}
/*系统初始GUI生成*/
void user_gui_init()
{	
	src=lv_scr_act();
	btn1=lv_btn_create(src,NULL);
	label2 = lv_label_create(btn1,NULL);
	label3=lv_label_create(src,NULL);
	image1 = lv_img_create(src,NULL);/* 创建 image 控件 */
	label1 = lv_label_create(src,NULL); /* 创建 label 控件 */

	lv_obj_set_size(btn1,100,50);
	lv_obj_set_pos(btn1,120,350);
	
	lv_btn_set_ink_in_time(btn1,500);
	lv_btn_set_ink_wait_time(btn1,1000);
	lv_btn_set_ink_out_time(btn1,500);	
	lv_obj_set_event_cb(btn1, btn1_event_cb);
	
	lv_label_set_text(label2,"Open"); /* 设置文字 */
	
	
	lv_img_set_src(image1,&temp_img); /* 为控件设置图片 */ 
	lv_obj_align(image1,NULL,LV_ALIGN_IN_TOP_MID,100,0);
	lv_obj_set_pos(image1,120,100);
	
	lv_style_copy(&style_cn, &lv_style_pretty_color); /* 复制 style 的属性 */
	style_cn.text.color = LV_COLOR_RED; /* 设置文字颜色 */
	style_cn.text.font = &Font;
	
	lv_obj_set_style(label3,&style_cn); /* 为控件设置新的 style */
	lv_obj_set_style(label1,&style_cn); /* 为控件设置新的 style */
	
	lv_label_set_text(label1,"2021嵌入式芯片与系统设计竞赛"); /* 设置文字 */
	lv_label_set_text(label3,"作品展示"); /* 设置文字 */
	
	lv_obj_align(label1,image1,LV_ALIGN_OUT_BOTTOM_MID,0,100);
	lv_obj_align(label3,label1,LV_ALIGN_OUT_BOTTOM_MID,0,0);
	
	lv_obj_align(btn1,label1,LV_ALIGN_OUT_BOTTOM_MID,0,50);
}


void gauge_gui()
{
	lv_obj_t *gauge_label1;
	
	lv_obj_t *gauge_label2;
	lv_obj_t *gauge_label3;
	lv_obj_t *gauge_label4;
	
	static lv_style_t style_desktop;
	static lv_style_t style_gauge1; /* 用于存储指针颜色的数组 */ 
	static lv_color_t needle_colors[2];
	
	needle_colors[0] = LV_COLOR_BLUE;
	needle_colors[1] = LV_COLOR_ORANGE;
	
	lv_style_copy(&style_desktop, &lv_style_scr);
	style_desktop.body.main_color = LV_COLOR_WHITE; /* 设置底色 */ /* 设置样式的字体颜色 */
	
	lv_obj_t *scr = lv_disp_get_scr_act(NULL); /* 获取当前屏幕 */
	lv_obj_set_style(scr, &style_desktop); /* 设置样式 */
	lv_style_copy(&style_gauge1, &lv_style_pretty_color);
	style_gauge1.body.main_color = lv_color_hex(0x228b22); /* 比例尺开始处的线条颜色 */
	style_gauge1.body.grad_color = lv_color_hex(0x4488bb); /* 比例尺末端的线条颜色(与主色的渐变) */ 
	style_gauge1.body.padding.left = 5; /* 行长 */
	style_gauge1.body.padding.inner = 2; /* 标签与比例线的距离 */
	style_gauge1.body.radius = 10; /* 针原点圆的半径 */
	style_gauge1.line.width = 5; /* 线宽 */
	style_gauge1.line.color = LV_COLOR_RED; /* 临界值之后的线条颜色 */
	style_gauge1.text.color = lv_color_hex(0x228b22); /* 文本颜色 */
	
	gauge1 = lv_gauge_create(scr, NULL); /* 创建 gauge 控件 */
	gauge2 = lv_gauge_create(scr, NULL); /* 创建 gauge 控件 */
	gauge3 = lv_gauge_create(scr, NULL); /* 创建 gauge 控件 */
	
	gauge_label2=lv_label_create(scr, NULL);
	gauge_label3=lv_label_create(scr, NULL);
	gauge_label4=lv_label_create(scr, NULL);
	
	lv_obj_set_style(gauge_label2,&style_cn); /* 为控件设置新的 style */
	lv_obj_set_style(gauge_label3,&style_cn); /* 为控件设置新的 style */
	lv_obj_set_style(gauge_label4,&style_cn); /* 为控件设置新的 style */
	
	lv_obj_align(gauge1, src, LV_ALIGN_IN_TOP_LEFT, 20,30); /* 设置位置 */
	lv_obj_align(gauge3, src, LV_ALIGN_IN_TOP_RIGHT, -20,30); /* 设置位置 */
	lv_obj_align(gauge2, src, LV_ALIGN_IN_TOP_MID, 0,170); /* 设置位置 */

	lv_label_set_text(gauge_label2,"酒精"); /* 设置文字 */
	lv_label_set_text(gauge_label3,"烟雾"); /* 设置文字 */
	lv_label_set_text(gauge_label4,"一氧化碳"); /* 设置文字 */
	
	
	lv_obj_align(gauge_label2, gauge1, LV_ALIGN_OUT_BOTTOM_MID, 0,0); /* 设置位置 */
	lv_obj_align(gauge_label3, gauge2, LV_ALIGN_OUT_BOTTOM_MID, 0,0); /* 设置位置 */
	lv_obj_align(gauge_label4, gauge3, LV_ALIGN_OUT_BOTTOM_MID, 0,0); /* 设置位置 */
	
	lv_gauge_set_value(gauge1, 0, 0); /* 设置值 */
	lv_gauge_set_critical_value(gauge1, 80); /* 设置临界值 */
	lv_gauge_set_style(gauge1, LV_GAUGE_STYLE_MAIN, &style_gauge1); /* 设置样式 */
	lv_gauge_set_scale(gauge1, 270, 16, 6); /* 设置刻度和标签 */
	
	lv_gauge_set_value(gauge2, 0, 0); /* 设置值 */
	lv_gauge_set_critical_value(gauge2, 80); /* 设置临界值 */
	lv_gauge_set_style(gauge2, LV_GAUGE_STYLE_MAIN, &style_gauge1); /* 设置样式 */
	lv_gauge_set_scale(gauge2, 270, 16, 6); /* 设置刻度和标签 */
	
	lv_gauge_set_value(gauge3, 0, 0); /* 设置值 */
	lv_gauge_set_critical_value(gauge3, 80); /* 设置临界值 */
	lv_gauge_set_style(gauge3, LV_GAUGE_STYLE_MAIN, &style_gauge1); /* 设置样式 */
	lv_gauge_set_scale(gauge3, 270, 16, 6); /* 设置刻度和标签 */
	
	gauge_btn1=lv_btn_create(scr,NULL);
	gauge_label1=lv_label_create(gauge_btn1,NULL);
	
	lv_obj_set_size(gauge_btn1,100,50);
	
	lv_obj_set_style(gauge_label1,&style_cn); /* 为控件设置新的 style */
	lv_label_set_text(gauge_label1,"Return"); /* 设置文字 */
	lv_obj_align(gauge_btn1,scr,LV_ALIGN_IN_BOTTOM_LEFT,0,0);
	lv_obj_set_event_cb(gauge_btn1,gauge_event_cb);	
	
}

void Works_gui()
{

	lv_obj_t *works_label1;
	lv_obj_t *works_label4;
	lv_obj_t *works_label5;
	lv_obj_t *works_label6;
	lv_obj_t *works_label7;
	lv_obj_t *works_label8;
	lv_obj_t *works_label9;
	
	static lv_style_t style_led;
	
	works_btn1=lv_btn_create(src,NULL);
	works_btn2=lv_btn_create(src,NULL);
	works_btn3=lv_btn_create(src,NULL);
	
	works_label1=lv_label_create(btn1,NULL);
	
	works_label4=lv_label_create(lv_scr_act(),NULL);		//x
	works_label5=lv_label_create(lv_scr_act(),NULL);		//y
	works_label6=lv_label_create(lv_scr_act(),NULL);		//id
	works_label7=lv_label_create(lv_scr_act(),NULL);		//机械臂运行状态
	
	works_label8=lv_label_create(works_btn2,NULL);
	works_label9=lv_label_create(works_btn3,NULL);
	
	image1 = lv_img_create(src,NULL);/* logo */
	
	lv_obj_set_size(works_btn1,200,100);
	lv_obj_set_pos(works_btn1,50,50);
	
	lv_obj_set_size(works_btn2,100,50);
	lv_obj_set_size(works_btn3,100,50);
	lv_obj_align(works_btn2,NULL,LV_ALIGN_IN_BOTTOM_RIGHT,0,0);
	lv_obj_align(works_btn3,NULL,LV_ALIGN_IN_BOTTOM_RIGHT,0,-70);
	
	lv_btn_set_ink_in_time(works_btn1,500);
	lv_btn_set_ink_wait_time(works_btn1,1000);
	lv_btn_set_ink_out_time(works_btn1,500);
	
	lv_obj_set_style(works_label1,&style_cn); /* 为控件设置新的 style */
	lv_obj_set_style(works_label7,&style_cn); /* 为控件设置新的 style */
	lv_obj_set_style(works_label8,&style_cn); /* 为控件设置新的 style */
	lv_obj_set_style(works_label9,&style_cn); /* 为控件设置新的 style */
	
	lv_label_set_text(works_label1,"传送带(开/关)"); /* 设置文字 */
	lv_label_set_text(works_label7,"机械臂运行状态"); /* 设置文字 */
	lv_label_set_text(works_label8,"Return"); /* 设置文字 */
	
	lv_label_set_text(works_label9,"传感器"); /* 设置文字 */
	

	lv_label_set_text(works_label4,"x:"); /* 设置文字 */
	lv_label_set_text(works_label5,"y:"); /* 设置文字 */
	lv_label_set_text(works_label6,"id:"); /* 设置文字 */
	
	lv_obj_set_pos(works_label4,100,200);
	lv_obj_align(works_label5,works_label4,LV_ALIGN_OUT_BOTTOM_LEFT,0,0);
	lv_obj_align(works_label6,works_label5,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
	
	lv_style_copy(&style_led, &lv_style_pretty_color);
	style_led.body.shadow.width = 5;
	style_led.body.radius = LV_RADIUS_CIRCLE;
	style_led.body.border.width = 3;
	style_led.body.border.opa = 30;
	style_led.body.main_color = lv_color_hsv_to_rgb(100, 200, 100);
	style_led.body.grad_color = lv_color_hsv_to_rgb(100, 200, 100);
	style_led.body.border.color = lv_color_hsv_to_rgb(100,200,100);
	style_led.body.shadow.color = lv_color_hsv_to_rgb(100, 200,100);
	
	led1 = lv_led_create(lv_scr_act(), NULL); /* 创建 led 控件 */
	led2 = lv_led_create(works_btn1, NULL); /* 创建 led 控件 */
	
	lv_led_set_style(led1, LV_LED_STYLE_MAIN, &style_led); /* 设置样式 */
	lv_led_set_style(led2, LV_LED_STYLE_MAIN, &style_led); /* 设置样式 */	
	
	lv_obj_set_pos(led1,210,305);
	lv_obj_set_pos(led2,30,15);
	lv_obj_set_pos(works_label1,0,0);
	lv_obj_set_pos(works_label7,40,300);
	
	lv_led_on(led1); /* 打开 led1 */
	lv_led_off(led2); /* 关闭 led2 */
	
	lv_img_set_src(image1,&temp_img); /* 为控件设置图片 */ 
	lv_obj_align(image1,NULL,LV_ALIGN_IN_TOP_MID,100,0);
	lv_obj_set_pos(image1,100,370);
	
	lv_obj_set_event_cb(works_btn1,works_btn1_cd);
	lv_obj_set_event_cb(works_btn2,works_btn1_cd);
	lv_obj_set_event_cb(works_btn3,works_btn1_cd);	
}
 


