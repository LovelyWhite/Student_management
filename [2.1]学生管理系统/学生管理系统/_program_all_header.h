#ifndef _PROGRAM_ALL_HEADER
#define _PROGRAM_ALL_HEADER
#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<conio.h>
#define EXIT 1
#define SUCCESS 0
#define FAIL -1
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define BOY 1
#define GIRL 0
#define CLS_SCREEN system("cls")
#define SYSTEM_PAUSE system("pause")
#define DELAY_MAX Sleep(750)
#define DELAY_MID Sleep(500)
#define DELAY_MIN Sleep(350)
#define credit_advanced_mathematics 5.5
#define credit_langue_programming 3.5
#define credit_data_struct 3.5
#define credit_college_English 2.0
#define credit_college_physics 3.0
#define credit_physical_education 1.0
#define credit_selective_course 1.0  
#define RING Beep(1000,200);
#define ADMIN 0
#define USER 1
HANDLE _handle;
COORD _coordinate;
CONSOLE_SCREEN_BUFFER_INFO _screen_info; //ȫ�ֱ������ڶ�λ���
typedef struct _Account
{
	char _password[20];
	int _authority;
}_account;//�˻�����
typedef struct _Account_node
{
	_account one_account;
	struct _Account_node * _next;
}_account_link;//�˻����
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
}_Score;//ѧ����������
typedef struct birthday
{
	int _year;
	int _month;
	int _day;
}_Birthday;//ѧ����������
typedef struct student
{
	char _name[20];//�9λ
	char _student_id[20];//�18λ
	char _school[40];//�19λ
	char _major[40];//�19λ
	char _class[14];//�6λ�ַ�
	int _sex;
	_Birthday _birthday;
	_Score _score;
}_student;//ѧ������
typedef struct _student_node
{
	_student one_student;
	struct _student_node* _next;
}_link;//ѧ�����
void _setting_interface();//���ý���
void _startup();//��������
void _edit_password_interface();//�༭�������
void _find_interface();//���������溯��
void _find_interface_id();//����ѧ�Ž���
void _find_interface_name();//���� ���� ����
void _find_interface_school();//���� ѧԺ ����
void _find_interface_major();//���� רҵ����
void _find_interface_class();//���� �༶����
void _main_interface();//�������˵�����
void _start_interface();//��ʼ ��ע���½���棩
void _fuzzy_search_interface_name();//����ģ�����ҽ���
void _fuzzy_search_interface_id();//ѧ��ģ�����ҽ���
void _del_interface(); //ɾ������
void _fuzzy_search_interface();//ģ�����ҽ���
void _password_system_interface();//����ϵͳ����
void _register_interface();//ע�����
void _register_interface_admin();
void _register_interface_user();//����Ա/��ͨ�û�ע�����
void _edit_admin_password_interface();
void _edit_user_password_interface();//�༭����Ա/��ͨ�û��������
void _user_up_interface();//�����û�Ȩ�޽���
void _encrypt_and_decrypt_password_system(_account_link *_account_head);//������ܽ��ܺ���
double _caculate_gpa(int, int, int, int, int, int, int);//���㼨�㺯������׼4.0������
void _putout_all_student_information(_link* _head_node, int _number_of_students);//�������ѧ����Ϣ����
void _putout_student_information(_link* _node, int _findout_number, int _now_number);//���ѧ����Ϣ����
int _start_function(_account_link** _account_head, int* _account_number);//��ʼ����
void _load_student_information(_link** _head_node, int *_number_of_students);//����ѧ����Ϣ����
void _find_student_information(_link* _head_node);//����ѧ����Ϣ����
void _add_student_information(_link** _head_node, int *_number_of_students);//���ѧ����Ϣ���� ѧ��������������Ϊ����������ݵ�ʱ�������ǰ�����û��ж�����Ϣ������������������ĸ���Ϣ
int _password_system(_account_link* _account_head);//������֤����
int _log_in(_account_link* _account_head, int _account_number);//��½ϵͳ
void _register(_account_link** _account_head, int* _account_number);//ע��ϵͳ
void _input_record(_link* _node, int _number_of_students, _link* _head_node);//�����¼����
void _save_student_information(_link* _head_node);//����ѧ����Ϣ����
int _fuzzy_compare(char* _origin, char * _textin);//ģ���ȽϺ���
void _bulk_load(_link** _head_node, int* _number_of_students);//�������빦��
int _if_have_ADMIN(_account_link *_account_head);
void _edit_student_information(_link* _head_node);//�༭ѧ����Ϣ����
void _edit_password(_account_link** _account_head, int* _account_number);//�༭���뺯��
void _edit_admin_password(_account_link** _account_head, int* _account_number);
void _edit_user_password(_account_link** _account_head, int* _account_number);
void _find_by_student_id(_link* _head_node);//ѧ�Ų��Һ���
void _find_by_name(_link* _head_node);//���ֲ��Һ���
void _find_by_school(_link* _head_node);//ѧԺ���Һ���
void _find_by_major(_link* _head_node);//רҵ���Һ���
void _find_by_class(_link* _head_node);//�༶���Һ���
void _fuzzy_search(_link* _head_node);//ģ�����Һ���
void _fuzzy_search_by_name(_link* _head_node);//����ģ�����Һ���
void _fuzzy_search_by_student_id(_link* _head_node);//ѧ��ģ�����Һ���
void _del_student_information(_link* _head_node, int *_number_of_students);//ɾ��ѧ����Ϣ����
_link* _find_by_string(_link* _head_node, char *_string, int _select_menu);//����ѧ����㺯��
int _findout_number(_link* _head_node, char *_string, int _select_menu);//���ز��ҵ���ѧ����������
void _load_account(_account_link** _account_head, int* _account_number);//�����˻���Ϣ����
void _save_account(_account_link* _account_head);//�����˻�����
void _register_authority(_account_link** _account_head, int* _account_number, int _authority);//��Ȩ��ע�ắ��
void _user_up(_account_link* _account_head);//�û�Ȩ����������
void _sort_student_information(_link* _head_node);//ѧ����Ϣ������
void _setting();
void _setting_initialize();
void _add_slash(char *_name);
void _setting_save();
#endif

