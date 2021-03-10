#ifndef _PROGRAM_ALL_HEADER
#define _PROGRAM_ALL_HEADER
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#define EXIT 1
#define SUCCESS 0
#define FAIL -1
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define BOY 1
#define GIRL 0
#define CLS_SCREEN system("clear")
#define DELAY_MAX 
#define DELAY_MID 
#define DELAY_MIN 
#define credit_advanced_mathematics 5.5
#define credit_langue_programming 3.5
#define credit_data_struct 3.5
#define credit_college_English 2.0
#define credit_college_physics 3.0
#define credit_physical_education 1.0
#define credit_selective_course 1.0
#define ADMIN 0
#define USER 1
typedef struct _Account
{
	char _password[20];
	int _authority;
} _account; // 账户类型
typedef struct _Account_node
{
	_account one_account;
	struct _Account_node *_next;
} _account_link; // 账户结点
typedef struct score
{
	int _advanced_mathematics;
	int _langue_programming;
	int _data_struct;
	int _college_English;
	int _college_physics;
	int _physical_education;
	int _selective_course;
	double _GPA;
} _Score; // 学生分数类型
typedef struct birthday
{
	int _year;
	int _month;
	int _day;
} _Birthday; // 学生生日类型
typedef struct student
{
	char _name[20];		  // 最长9位
	char _student_id[20]; // 最长18位
	char _school[40];	  // 最长19位
	char _major[40];	  // 最长19位
	char _class[14];	  // 最长6位字符
	int _sex;
	_Birthday _birthday;
	_Score _score;
} _student; // 学生类型
typedef struct _student_node
{
	_student one_student;
	struct _student_node *_next;
} _link;							 // 学生结点
void _setting_interface();			 // 设置界面
void _startup();					 // 启动界面
void _edit_password_interface();	 // 编辑密码界面
void _find_interface();				 // 查找主界面函数
void _find_interface_id();			 // 查找学号界面
void _find_interface_name();		 // 查找 名字 界面
void _find_interface_school();		 // 查找 学院 界面
void _find_interface_major();		 // 查找 专业界面
void _find_interface_class();		 // 查找 班级界面
void _main_interface();				 // 主函数菜单界面
void _start_interface();			 // 开始 （注册登陆界面）
void _fuzzy_search_interface_name(); // 名字模糊查找界面
void _fuzzy_search_interface_id();	 // 学号模糊查找界面
void _del_interface();				 // 删除界面
void _fuzzy_search_interface();		 // 模糊查找界面
void _password_system_interface();	 // 密码系统界面
void _register_interface();			 // 注册界面
void _register_interface_admin();
void _register_interface_user(); // 管理员/普通用户注册界面
void _edit_admin_password_interface();
void _edit_user_password_interface();												  // 编辑管理员/普通用户密码界面
void _user_up_interface();															  // 提升用户权限界面
void _encrypt_and_decrypt_password_system(_account_link *_account_head);			  // 密码加密解密函数
double _caculate_gpa(int, int, int, int, int, int, int);							  // 计算绩点函数（标准4.0方案）
void _putout_all_student_information(_link *_head_node, int _number_of_students);	  // 输出所有学生信息函数
void _putout_student_information(_link *_node, int _findout_number, int _now_number); // 输出学生信息函数
int _start_function(_account_link **_account_head, int *_account_number);			  // 开始函数
void _load_student_information(_link **_head_node, int *_number_of_students);		  // 载入学生信息函数
void _find_student_information(_link *_head_node);									  // 查找学生信息函数
void _add_student_information(_link **_head_node, int *_number_of_students);		  // 添加学生信息函数 学生数量的作用是为了在输出数据的时候可以提前告诉用户有多少信息，现在正在浏览的是哪个信息
int _password_system(_account_link *_account_head);									  // 密码验证函数
int _log_in(_account_link *_account_head, int _account_number);						  // 登陆系统
void _register(_account_link **_account_head, int *_account_number);				  // 注册系统
void _input_record(_link *_node, int _number_of_students, _link *_head_node);		  // 输入记录函数
void _save_student_information(_link *_head_node);									  // 保存学生信息函数
int _fuzzy_compare(char *_origin, char *_textin);									  // 模糊比较函数
void _bulk_load(_link **_head_node, int *_number_of_students);						  // 批量导入功能
int _if_have_ADMIN(_account_link *_account_head);
void _edit_student_information(_link *_head_node);						  // 编辑学生信息函数
void _edit_password(_account_link **_account_head, int *_account_number); // 编辑密码函数
void _edit_admin_password(_account_link **_account_head, int *_account_number);
void _edit_user_password(_account_link **_account_head, int *_account_number);
void _find_by_student_id(_link *_head_node);												   // 学号查找函数
void _find_by_name(_link *_head_node);														   // 名字查找函数
void _find_by_school(_link *_head_node);													   // 学院查找函数
void _find_by_major(_link *_head_node);														   // 专业查找函数
void _find_by_class(_link *_head_node);														   // 班级查找函数
void _fuzzy_search(_link *_head_node);														   // 模糊查找函数
void _fuzzy_search_by_name(_link *_head_node);												   // 名字模糊查找函数
void _fuzzy_search_by_student_id(_link *_head_node);										   // 学号模糊查找函数
void _del_student_information(_link *_head_node, int *_number_of_students);					   // 删除学生信息函数
_link *_find_by_string(_link *_head_node, char *_string, int _select_menu);					   // 查找学生结点函数
int _findout_number(_link *_head_node, char *_string, int _select_menu);					   // 返回查找到的学生数量函数
void _load_account(_account_link **_account_head, int *_account_number);					   // 载入账户信息函数
void _save_account(_account_link *_account_head);											   // 保存账户函数
void _register_authority(_account_link **_account_head, int *_account_number, int _authority); // 以权限注册函数
void _user_up(_account_link *_account_head);												   // 用户权限提升函数
void _sort_student_information(_link *_head_node);											   // 学生信息排序函数
void _move_cursor(int x, int y);
void _setting();
void _setting_initialize();
void _add_slash(char *_name);
void _setting_save();
#endif
