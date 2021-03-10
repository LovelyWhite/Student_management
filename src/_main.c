#include"_program_all_header.h"
char _student_file_name[200];
char _account_file_name[200];
char _bluk_import_file_name[200];
int AUTHORITY = USER;
int main(void)
{
	printf("\e[8;16;93t");//定义宽和高
	_startup();
	_setting_initialize();
	// while (_kbhit())
	// {
	// 		char ch1 = getchar();
	// 		char ch2 = getchar();
	// 		if (ch1 == 0 && ch2 == 60)
	// 		{
	// 			_setting(); goto LOG_IN;
	// 		}
	// 		else
	// 			break;
	// }
	int _number_of_students = 0, _judge = 0;
	char _select = '*';
	int _account_number = 0;
LOG_IN:
	_setting_initialize();
	_number_of_students = 0, _account_number = 0;
	_account_link* _account_head = NULL;
	_load_account(&_account_head, &_account_number);
	AUTHORITY = USER;//初始化权限
	_judge = _start_function(&_account_head, &_account_number);
	if (_judge == FAIL || _judge == EXIT)
	{
		return 0;
	}
	else
	{
		_number_of_students = 0;                        //每次载入之前初始化学生数量为0
		_link *_head_node = NULL;
		_load_student_information(&_head_node, &_number_of_students);//用二级指针的目的是为了改变该指针的值
		_main_interface();
		_select = getchar();
		if (_select == ESC)
		{
			if (_head_node->_next != NULL)//程序退出到登陆界面时 释放掉所有申请的内存
			{
				free(_head_node->_next);
			}
			goto LOG_IN;
		}
		_select -= '0';
		while (1)
		{
			switch (_select)
			{
			case 1:_find_student_information(_head_node); break;
			case 2:_add_student_information(&_head_node, &_number_of_students); break;
			case 3:_del_student_information(_head_node, &_number_of_students); break;
			case 4:_edit_student_information(_head_node); break;
			case 5:_putout_all_student_information(_head_node, _number_of_students); break;
			case 6:_edit_password(&_account_head, &_account_number); break;
			case 7:_user_up(_account_head); break;
			case 8:_bulk_load(&_head_node, &_number_of_students); break;
			case 9:_sort_student_information(_head_node); break;
			case 'S' - '0':_setting(); break;
			default:;
			}
			_main_interface();
			_select = getchar();
			if (_select == ESC)
			{
				goto LOG_IN;//这里用goto 语句来快速退出2层循环
				while (_head_node->_next != NULL)//程序退出到登陆界面时 释放掉所有申请的内存
				{
					free(_head_node->_next);
				}
				free(_head_node);
				while (_account_head->_next != NULL)
				{
					free(_account_head->_next);
				}
				free(_account_head);
			}
			_select -= '0';
		}
	}
}