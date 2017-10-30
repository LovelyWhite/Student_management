#include"_program_all_header.h"
extern char _student_file_name[200];
extern char  _start_password[20];
extern char _account_file_name[200];
extern char _bluk_import_file_name[200];
extern int AUTHORITY;
void _load_student_information(_link** _head_node, int *_number_of_students)     //用二级指针的目的是为了改变该指针的值
{
	(*_head_node) = (_link*)malloc(sizeof(_link));
	(*_head_node)->_next = NULL;
	int _error_store = 0;
	char _first_char = '*';
	FILE *_fileptr = NULL;
	_error_store = fopen_s(&_fileptr, _student_file_name, "rb");
	if (_error_store == 0)
	{
		_first_char = fgetc(_fileptr);
		fclose(_fileptr);
	}
	if (_error_store != 0 || _first_char == EOF)
	{
		CLS_SCREEN;
		printf(" __________________________________________________________________________________________\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                      无学生信息                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|__________________________________________________________________________________________|\n");
		DELAY_MAX;
	}
	else
	{
		fopen_s(&_fileptr, _student_file_name, "rb");
		_link* _node = NULL;
		while (ferror(_fileptr) == 0 && feof(_fileptr) == 0)
		{
			_node = (_link*)malloc(sizeof(_link));
			fread(&(_node->one_student), sizeof(_student), 1, _fileptr);
			_node->_next = (*_head_node)->_next;
			(*_head_node)->_next = _node;
			(*_number_of_students)++;
		}
		fclose(_fileptr);
		if ((*_head_node)->_next != NULL)//为了解决 那个多读取一个的错误 只能让头指针 往后移动一位了
		{
			(*_head_node) = (*_head_node)->_next;
			(*_number_of_students)--;
		}
	}
}
void _find_student_information(_link* _head_node)
{
	char _select_menu = '*';
	_find_interface();
	_select_menu = _getch();
	if (_select_menu == ESC)
	{
	MAIN_INTERFACE:
		return;
	}
	else
	{
		_select_menu -= '0';
		while (1)
		{
			switch (_select_menu)
			{
			case 1:_find_by_student_id(_head_node);break;
			case 2:_find_by_name(_head_node);break;
			case 3:_find_by_school(_head_node);break;
			case 4:_find_by_major(_head_node);break;
			case 5:_find_by_class(_head_node);break;
			case 6:_fuzzy_search(_head_node);break;
			default:RING;
			}
			_find_interface();
			_select_menu = _getch();
			if (_select_menu == ESC)
			{
				goto MAIN_INTERFACE;
			}
			_select_menu -= '0';
		}
	}
}
void _add_student_information(_link** _head_node, int *_number_of_students)
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	char _select_judge = 'Y';
	if (AUTHORITY == ADMIN)
	{
		_link* _node = (_link*)malloc(sizeof(_link));
		while (_select_judge == 'Y' || _select_judge == 'y')
		{
			_main_interface();
			_input_record(_node, *_number_of_students, *_head_node);
			_node->_next = (*_head_node)->_next;
			(*_head_node)->_next = _node;
			_main_interface();
			_coordinate.X = 25; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
			_save_student_information(*_head_node);
			(*_number_of_students)++;
			while (1)
			{
				_main_interface();
				_coordinate.X = 25; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
				printf("是否继续添加（Y/N）");
				_select_judge = _getch();
				if (_select_judge == 'N' || _select_judge == 'n') goto MAIN_INTERFACE;
				else if (_select_judge == 'Y' || _select_judge == 'y')break;
			}
		}
	}
	else
	{
		_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
		printf("当前权限不足!"); DELAY_MAX;
	MAIN_INTERFACE:;
	}
}
void _del_student_information(_link* _head_node, int *_number_of_students)
{
	char _id[20];
	char _select_judge = 'N';
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	if (AUTHORITY == ADMIN)
	{
		_del_interface();
		_link *_found = _head_node, *_copy = NULL;
		_coordinate.X = 20; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
		scanf_s("%s", _id, 20);
		while (_found->_next != NULL)
		{
			if (strcmp(_found->_next->one_student._student_id, _id) == 0)
				break;
			_found = _found->_next;
		}
		if (_found->_next != NULL)
		{
			_putout_student_information(_found->_next, 1, 1);
			printf("已找到按D删除!");
			while (1)
			{
				_select_judge = _getch();
				if (_select_judge == 'D' || _select_judge == 'd')
				{
					if (_found->_next->_next != NULL)
					{
						_copy = _found->_next->_next;
						free(_found->_next);
						_found = _copy;
					}
					else
					{
						free(_found->_next);
						_found->_next = NULL;
					}
					(*_number_of_students)--;
					_save_student_information(_head_node);
				}
				else if (_select_judge == ESC)
					return;
			}
		}
		else
		{
			_coordinate.X = 20; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			printf("无此学生信息!"); DELAY_MAX;
		}

	}
	else
	{
		_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
		printf("当前权限不足!"); DELAY_MAX;
	}
}
void _edit_student_information(_link* _head_node)
{
	if (AUTHORITY == ADMIN)
	{
		char _key = '*';
		char _strcmp_sex[3];
		_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(_handle, &_screen_info);
		char _id[20];
		_link* _findout = NULL;
		_coordinate.X = 8; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate);
		printf(":请输入学生的学号(不多于18位):");
		scanf_s("%s", _id, 20);
		_findout = _find_by_string(_head_node, _id, 0);
		if (_findout != NULL)
		{
			_putout_student_information(_findout, 1, 1);
			_coordinate.X = 24; _coordinate.Y = 12; SetConsoleCursorPosition(_handle, _coordinate);
			printf("按E键编辑");
			while (1)
			{
				_key = _getch();
				if (_key == ESC)
				{
					goto MAIN_INTERFACE;
				}
				else if (_key ==  ENTER)
				{
					goto start;
				}
			}
			start:_coordinate.X = 11; _coordinate.Y = 3; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto a;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf_s("%s", _findout->one_student._name, 20);
			}
		a: _coordinate.X = 11; _coordinate.Y = 4; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto SEX;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf_s("%s", _findout->one_student._student_id, 20);
			}
		SEX:_coordinate.X = 11; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto b;
			}
			else
			{
			_SEX:printf("        \b\b\b\b\b\b\b\b");
				scanf_s("%s", _strcmp_sex, 3);
			}
			if (strcmp("男", _strcmp_sex) == 0)
				_head_node->one_student._sex = 1;
			else if (strcmp("女", _strcmp_sex) == 0)
				_head_node->one_student._sex = 0;
			else
				goto _SEX;
		b:_coordinate.X = 11; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto c;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf_s("%s", _findout->one_student._school, 40);
			}
		c:_coordinate.X = 11; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto d;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf_s("%s", _findout->one_student._major, 40);
			}
		d:_coordinate.X = 11; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto e;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf_s("%s", _findout->one_student._class, 14);
			}
		e:_coordinate.X = 11; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto f;
			}
			else
			{
				printf("    \b\b\b\b");
				scanf_s("%d", &_findout->one_student._birthday._year);
			}
		f:_coordinate.X = 17; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto g;
			}
			else
			{
				printf("  \b\b");
				scanf_s("%d", &_findout->one_student._birthday._month);
			}
		g:_coordinate.X = 21; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto h;
			}
			else
			{
				printf("  \b\b");
				scanf_s("%d", &_findout->one_student._birthday._day);
			}
		h:_coordinate.X = 76; _coordinate.Y = 3; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto i;
			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._advanced_mathematics);
			}
		i:_coordinate.X = 76; _coordinate.Y = 4; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto j;
			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._langue_programming);
			}
		j:_coordinate.X = 76; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate); 
			_key = _getch();
			if (_key == ENTER)
			{
				goto k;
			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._data_struct);
			}
		k:_coordinate.X = 76; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto l;
			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._college_English);
			}
		l:_coordinate.X = 76; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto m;
			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._college_physics);
			}
		m:_coordinate.X = 76; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{
				goto n;
			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._physical_education);
			}
		n:_coordinate.X = 76; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
			_key = _getch();
			if (_key == ENTER)
			{

			}
			else
			{
				printf("   \b\b\b");
				scanf_s("%d", &_findout->one_student._score._selective_course);
			}
			_head_node->one_student._score._GPA = _caculate_gpa(_findout->one_student._score._advanced_mathematics, _findout->one_student._score._langue_programming, _findout->one_student._score._data_struct, _findout->one_student._score._college_English, _findout->one_student._score._physical_education, _findout->one_student._score._physical_education, _findout->one_student._score._selective_course);
			_main_interface();
			_coordinate.X = 30; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
			_save_student_information(_head_node);
		}
		else
		{
			_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
			printf("未找到该学号学生!"); DELAY_MAX; MAIN_INTERFACE:;
		}
	}
	else
	{
		_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
		printf("当前权限不足!"); DELAY_MAX;
	}
}
void _putout_all_student_information(_link* _head_node, int _number_of_students)
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	char _first_char = '*';
	int _error_store = 0;
	FILE* _fileptr = NULL;
	_error_store = fopen_s(&_fileptr, _student_file_name, "rb");
	if (_error_store == 0)
	{
		_first_char = fgetc(_fileptr);
		fclose(_fileptr);
	}
	if (_error_store != 0 || _first_char == EOF)
	{
		_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
		printf("无学生信息，请添加!"); DELAY_MAX; MAIN_INTERFACE:;
	}
	else
	{
		char _judge = '*';
		int i = 1;
		while (_head_node->_next != NULL)
		{
			_putout_student_information(_head_node->_next, _number_of_students, i);
			i++;
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					_head_node = _head_node->_next;
					break;
				}
				else if (_judge == ESC)
					goto MAIN_INTERFACE;//退出循环
			}
		}
	}
}
void _edit_password(_account_link** _account_head, int* _account_number)
{
	char _select_menu = '*';
	_edit_password_interface();
	_select_menu = _getch();
	if (_select_menu == ESC)
	{
	MAIN_INTERFACE:return;
	}
	_select_menu -= '0';
	while (1)
	{
		switch (_select_menu)
		{
		case 1:_edit_admin_password(_account_head, _account_number); break;
		case 2:_edit_user_password(_account_head, _account_number); break;
		default:RING;
		}
		_edit_password_interface();
		_select_menu = _getch();
		if (_select_menu == ESC)
		{
			goto MAIN_INTERFACE;
		}
		_select_menu -= '0';
	}
}
int _password_system(_account_link* _account_head)                                               //密码正确返回 SUCCESS 否则返回 FAIL
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	char _password_judge[20];
	int i = 0;
LOGIN:_password_system_interface();
	_coordinate.X = 36; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
	while (_password_judge[i] = _getch(), _password_judge[i] != ENTER)
	{
		if (_password_judge[i] == ESC)return EXIT;
		if (i == 0 && _password_judge[i] == BACKSPACE)
		{
			RING;
			_password_judge[i] = '\0';
			goto LOGIN;
		}

		else if (_password_judge[i] == BACKSPACE)
		{
		DEL:i--;
			printf("\b \b");
		}
		else
		{
			i++;
			printf("*");
			if (i == 19)
			{
				RING;
				goto DEL;
			}
		}
	}
	if (i == 0 && _password_judge[i] == ENTER)//放在while 循环外的原因是while循环是 以enter结束的
	{
		RING;
		goto LOGIN;
	}
	_password_judge[i] = '\0';
	while (_account_head->_next != NULL)
	{
		if (strcmp(_account_head->_next->one_account._password, _password_judge) != 0)
		{
			_account_head = _account_head->_next;

		}
		else
		{
			if (_account_head->_next->one_account._authority != ADMIN)
			{
				_coordinate.X = 58; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
				printf("权限不足!"); DELAY_MAX;
				return FAIL;
			}
			else
			{
				_coordinate.X = 58; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
				printf("欢迎您管理员!"); DELAY_MID;
				return SUCCESS;
			}
		}
	}
	_coordinate.X = 58; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
	printf("密码错误请重试!"); DELAY_MAX; return FAIL;
}
int _log_in(_account_link* _account_head, int _account_number)
{
	_account_link* _account_head_copy = _account_head;
	int _errtime = 0;
	char _password_judge[20];
	int i = 0;
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);//初始化光标定位
	GetConsoleScreenBufferInfo(_handle, &_screen_info);//初始化光标定位
	if (_account_number == 0)
	{
		CLS_SCREEN;
		printf(" __________________________________________________________________________________________\n"
			"|                                                                                          |\n"
			"|    1.注册                                                                                |\n"
			"|    2.登陆                                                           DOS学生管理系统      |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|         无密码请先创建密码                                                               |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|   copyright  LovelyWhite. ALL RIGITS RESERVED                                     ESC退出|\n"
			"|__________________________________________________________________________________________|\n");; DELAY_MAX;
		return EXIT;
	}
	else
	{
	LOGIN:CLS_SCREEN;
		printf(" __________________________________________________________________________________________\n"
			"|                                                                                          |\n"
			"|    1.注册    ____________________                                                        |\n"
			"|    2.登陆-->|                    |                                  DOS学生管理系统      |\n"
			"|             |____________________|  剩余%1d次机会                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|                                                                                          |\n"
			"|   copyright  LovelyWhite. ALL RIGITS RESERVED                                     ESC退出|\n"
			"|__________________________________________________________________________________________|\n", 3 - _errtime);
		_coordinate.X = 15; _coordinate.Y = 3; SetConsoleCursorPosition(_handle, _coordinate);
		i = 0;
		while (_password_judge[i] = _getch(), _password_judge[i] != ENTER)
		{
			if (_password_judge[i] == ESC)
			{
				return EXIT;
			}
			if (i == 0 && _password_judge[i] == BACKSPACE)
			{
				RING;
				_password_judge[i] = '\0';
				goto LOGIN;
			}

			else if (_password_judge[i] == BACKSPACE)
			{
			DEL:i--;
				printf("\b \b");
			}
			else
			{
				i++;
				printf("*");
				if (i == 19)
				{
					RING;
					goto DEL;
				}
			}
		}
		if (i == 0 && _password_judge[i] == ENTER)
		{
			RING;
			goto LOGIN;
		}
		_password_judge[i] = '\0';
		while (_account_head_copy->_next != NULL)
		{
			if (strcmp(_account_head_copy->_next->one_account._password, _password_judge) != 0)
			{
				_account_head_copy = _account_head_copy->_next;

			}
			else
			{
				if (_account_head_copy->_next->one_account._authority == ADMIN)
					AUTHORITY = ADMIN;
				_coordinate.X = 24; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
				printf("欢迎使用!"); DELAY_MID;
				return SUCCESS;
			}
		}
		if (_errtime == 2)
		{
			_coordinate.X = 24; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			printf("尝试次数超过三次，系统即将退出!"); DELAY_MAX; DELAY_MAX; DELAY_MAX; DELAY_MAX;
			return FAIL;
		}
		else
		{
			_coordinate.X = 24; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			printf("密码错误请重试!"); DELAY_MAX;
			_account_head_copy = _account_head;
			_errtime++;
			goto LOGIN;
		}

	}

}
void _register(_account_link** _account_head, int* _account_number)
{
	char _select_menu = '*';
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	if (_if_have_ADMIN(*_account_head) == FAIL)
	{
		_coordinate.X = 25; _coordinate.Y = 2; SetConsoleCursorPosition(_handle, _coordinate);
		printf("请先创建一个管理员密码!"); DELAY_MAX;
		goto REG;
	}
	else
	{
		_register_interface();
		_select_menu = _getch();
		if (_select_menu == ESC)
		{
			return;
		}
		else
		{
			_select_menu -= '0';
			while (1)
			{
				AUTHORITY = USER;//初始化权限
				switch (_select_menu)
				{
				REG:case 1:_register_authority(_account_head, _account_number, ADMIN); break;
				case 2:
					if (_if_have_ADMIN(*_account_head) == FAIL)//用于防止一个系统bug
					{
						_coordinate.X = 25; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
						printf("请先创建一个管理员密码!"); DELAY_MAX;
						goto REG;
					}
					else
						_register_authority(_account_head, _account_number, USER); break;
				default:RING;
				}
				_register_interface();
				_select_menu = _getch();
				if (_select_menu == ESC)
					return;
				else
					_select_menu -= '0';
			}
		}
	}

}
int _start_function(_account_link** _account_head, int* _account_number)
{

	int _judge = 0;
	char _select_menu = '*';
	_start_interface();
	_select_menu = _getch();
	if (_select_menu == ESC)
	{
		return EXIT;
	}
	else
	{
		_select_menu -= '0';
		while (1)
		{
			switch (_select_menu)
			{
			case 1:_register(_account_head, _account_number);  break;
			case 2:_judge = _log_in(*_account_head, *_account_number);
				if (_judge == SUCCESS)
				{
					return SUCCESS;
				}
				else if (_judge == FAIL)
				{
					return FAIL;
				}
				else  //EXIT 这个else是回到主界面
				{
					break;
				}
			default:RING;
			}
			_start_interface();
			_select_menu = _getch();
			if (_select_menu == ESC)
			{
				return EXIT;
			}
			_select_menu -= '0';
		}
	}
}
void _input_record(_link* _node, int _number_of_students, _link* _head_node)
{
	char _strcmp_sex[3];
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	system("cls");
	printf(" __________________________________________________________________________________________\n"
		"|                                                                                          |\n"
		"|                                                             成绩                         |\n"
		"|    姓名:                                                         高等数学:               |\n"
		"|    学号:                                                         程序设计:               |\n"
		"|    性别:                                                         数据结构:               |\n"
		"|    学院:                                                         大学英语:               |\n"
		"|    专业:                                                         大学物理:               |\n"
		"|    班级:                                                         体育    :               |\n"
		"|    生日:     年  月  日                                          选修课  :               |\n"
		"|                                                                  绩点    :               |\n"
		"|                                                                                          |\n"
		"|   正在添加第%3d名学生                ENTER继续                                           |\n"
		"|__________________________________________________________________________________________|\n", _number_of_students + 1);
	_coordinate.X = 10; _coordinate.Y = 3; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%s", _node->one_student._name, 20);
ID:_coordinate.X = 10; _coordinate.Y = 4; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%s", _node->one_student._student_id, 20);
	if (0 < _findout_number(_head_node, _node->one_student._student_id, 0))
	{
		RING;
		goto ID;
	}
SEX:_coordinate.X = 10; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%s", _strcmp_sex, 3);
	if (strcmp("男", _strcmp_sex) == 0)
		_node->one_student._sex = 1;
	else if (strcmp("女", _strcmp_sex) == 0)
		_node->one_student._sex = 0;
	else
	{
		goto SEX;
	}
	_coordinate.X = 10; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%s", _node->one_student._school, 40);
	_coordinate.X = 10; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%s", _node->one_student._major, 40);
	_coordinate.X = 10; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%s", _node->one_student._class, 14);
	_coordinate.X = 11; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._birthday._year);
	_coordinate.X = 17; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._birthday._month);
	_coordinate.X = 21; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._birthday._day);
	_coordinate.X = 76; _coordinate.Y = 3; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._advanced_mathematics);
	_coordinate.X = 76; _coordinate.Y = 4; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._langue_programming);
	_coordinate.X = 76; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._data_struct);
	_coordinate.X = 76; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._college_English);
	_coordinate.X = 76; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._college_physics);
	_coordinate.X = 76; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._physical_education);
	_coordinate.X = 76; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate); scanf_s("%d", &_node->one_student._score._selective_course);
	_node->one_student._score._GPA = _caculate_gpa(_node->one_student._score._advanced_mathematics, _node->one_student._score._langue_programming, _node->one_student._score._data_struct, _node->one_student._score._college_English, _node->one_student._score._physical_education, _node->one_student._score._physical_education, _node->one_student._score._selective_course);
}
void _save_student_information(_link* _head_node)
{
	FILE* _fileptr;
	if (fopen_s(&_fileptr, _student_file_name, "wb") != 0)
	{
		printf("储存失败请重试!"); DELAY_MAX;
		return;
	}
	else
	{
		while (_head_node->_next != NULL)
		{
			fwrite(&(_head_node->_next->one_student), sizeof(_student), 1, _fileptr);
			_head_node = _head_node->_next;
		}
		fclose(_fileptr);
		printf("保存成功!"); DELAY_MID;
	}
}
int _fuzzy_compare(char* _origin, char * _textin)
{
	int _count_origin = 0, _count_textin = 0, i, j, _err = 0;
	_count_origin = strlen(_origin);
	_count_textin = strlen(_textin);
	if (_count_textin > _count_origin)
	{
		return FAIL;
	}
	else
	{
		for (i = 0; i < _count_origin - _count_textin + 1; i++)
		{
			_err = 0;//放在这里的原因是为了不影响下一次推移的比较
			for (j = 0; j < _count_textin; j++)
			{
				if (_textin[j] == _origin[j + i])
				{
					_err++;
				}
			}
			if (_err == _count_textin)//一比较完就判断是否相同
			{
				return SUCCESS;
			}
			else
				return FAIL;
		}

	}
}
void _find_by_student_id(_link* _head_node)
{
	int _now_number = 0;
	_link* _findout = NULL;
	int _findout_sign = FAIL;
	char _id[20], _judge = '*';
	_find_interface_id();
	_coordinate.X = 28; _coordinate.Y = 4; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _id, 20);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _id, 0);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, 1, 1);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 53; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _find_by_name(_link* _head_node)
{
	int _found_number = 0, _now_number = 0;
	_link* _findout;
	int _findout_sign = FAIL;
	char _name[40], _judge = '*';
	_find_interface_name();
	_coordinate.X = 28; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _name, 40);
	_found_number = _findout_number(_head_node, _name, 1);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _name, 1);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 53; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}//输入问题
void _find_by_school(_link* _head_node)
{
	int _found_number = 0, _now_number = 0;
	_link* _findout;
	int _findout_sign = FAIL;
	char _school[40], _judge = '*';
	_find_interface_school();
	_coordinate.X = 28; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _school, 40);
	_found_number = _findout_number(_head_node, _school, 2);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _school, 2);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 53; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _find_by_major(_link* _head_node)
{
	int _found_number = 0, _now_number = 0;
	_link* _findout;
	int _findout_sign = FAIL;
	char _major[40], _judge = '*';
	_find_interface_major();
	_coordinate.X = 28; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _major, 40);
	_found_number = _findout_number(_head_node, _major, 3);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _major, 3);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 53; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _find_by_class(_link* _head_node)
{
	int _found_number = 0, _now_number = 0;
	_link* _findout;
	int _findout_sign = FAIL;
	char _class[40], _judge = '*';
	_find_interface_class();
	_coordinate.X = 28; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _class, 40);
	_found_number = _findout_number(_head_node, _class, 4);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _class, 4);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 53; _coordinate.Y = 5; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _fuzzy_search(_link* _head_node)
{
	char _select_menu = '*';
	_fuzzy_search_interface();
	_select_menu = _getch();
	if (_select_menu == ESC)
	{
	FIND_INTERFACE:_find_interface();
		return;
	}
	else
	{
		_select_menu -= '0';
		while (1)
		{
			switch (_select_menu)
			{
			case 1:_fuzzy_search_by_name(_head_node);               _fuzzy_search_interface(); break;
			case 2:_fuzzy_search_by_student_id(_head_node);         _fuzzy_search_interface(); break;
			default:RING;
			}
			_select_menu = _getch();
			if (_select_menu == ESC)
			{
				_find_interface();
				goto FIND_INTERFACE;
			}
			_select_menu -= '0';
		}
	}
}
void _fuzzy_search_by_name(_link* _head_node)
{
	int _found_number = 0, _now_number = 0;
	_link* _findout;
	int _findout_sign = FAIL;
	char _name[20], _judge = '*';
	_fuzzy_search_interface_name();
	_coordinate.X = 48; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _name, 20);
	_found_number = _findout_number(_head_node, _name, 6);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _name, 6);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 62; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _fuzzy_search_by_student_id(_link* _head_node)
{
	int _found_number = 0, _now_number = 0;
	_link* _findout;
	int _findout_sign = FAIL;
	char _id[20], _judge = '*';
	_fuzzy_search_interface_id();
	_coordinate.X = 48; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
	scanf_s("%s", _id, 20);
	_found_number = _findout_number(_head_node, _id, 5);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _id, 5);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = _getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					break;
				}
				else if (_judge == ESC)
				{
					goto FIND_INTERFACE;
				}
				_judge = _getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_coordinate.X = 62; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
		printf("未找到该学生"); DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _encrypt_and_decrypt_password_system(_account_link *_account_head)
{
	int i = 0;
	char key[] = { 'a','4', 'c', 'E', 'b', 'C', 'K', 'w', 'V','b','Y','n','J','e','Y','j','l','0','v' };
	for (i = 0; i<20; i++)
		(_account_head->one_account._password[i]) ^= key[i];
}
void _putout_student_information(_link* _node, int _findout_number, int _now_number)
{
	char __sex[4];
	if (_node->one_student._sex == 1)
	{
		strcpy_s(__sex, 4, "男");
	}
	else
	{
		strcpy_s(__sex, 4, "女");
	}
	system("cls");
	printf(" __________________________________________________________________________________________\n"
		"|                                                                                          |\n"
		"|                                                             成绩                         |\n"
		"|    姓名：%-9s                                               高等数学:%03d            |\n"
		"|    学号：%-18s                                      程序设计:%03d            |\n"
		"|    性别：%s                                                      数据结构:%03d            |\n"
		"|    学院：%-38s                  大学英语:%03d            |\n"
		"|    专业：%-38s                  大学物理:%03d            |\n"
		"|    班级：%-12s                                            体育    :%03d            |\n"
		"|    生日：%4d年%02d月%02d日                                          选修课  :%03d            |\n"
		"|                                                                  绩点    :%2.1lf            |\n"
		"|                                                                                          |\n"
		"|   共找到(%3d/%3d)个                  ENTER继续                                    ESC退出|\n"
		"|__________________________________________________________________________________________|\n", _node->one_student._name, _node->one_student._score._advanced_mathematics, _node->one_student._student_id, _node->one_student._score._langue_programming, __sex, _node->one_student._score._data_struct, _node->one_student._school, _node->one_student._score._college_English, _node->one_student._major, _node->one_student._score._college_physics, _node->one_student._class, _node->one_student._score._physical_education, _node->one_student._birthday._year, _node->one_student._birthday._month, _node->one_student._birthday._day, _node->one_student._score._selective_course, _node->one_student._score._GPA, _now_number, _findout_number);
}
double _caculate_gpa(int _advanced_mathematics, int _langue_programming, int _data_struct, int _college_English, int _college_physics, int _physical_education, int _selective_course)
{
	int i = 0, _score = 0;
	double _GPA = 0, _all_credit = 0, _credit = 0, _intermediate_quantity = 0;
	_all_credit = credit_advanced_mathematics + credit_langue_programming + credit_data_struct + credit_college_English + credit_college_physics + credit_physical_education + credit_selective_course;
	for (i = 0; i < 7; i++)
	{
		switch (i)
		{
		case 0: _score = _advanced_mathematics; _credit = credit_advanced_mathematics; break;
		case 1: _score = _langue_programming; _credit = credit_langue_programming; break;
		case 2: _score = _data_struct; _credit = credit_data_struct; break;
		case 3: _score = _college_English; _credit = credit_college_English; break;
		case 4: _score = _college_physics; _credit = credit_college_physics; break;
		case 5: _score = _physical_education; _credit = credit_physical_education; break;
		case 6: _score = _selective_course; _credit = credit_selective_course; break;
		}
		if (_score >= 90)
			_intermediate_quantity += 4.0*_credit;
		else if (_score >= 80)
			_intermediate_quantity += 3.0*_credit;
		else if (_score >= 70)
			_intermediate_quantity += 2.0*_credit;
		else if (_score >= 60)
			_intermediate_quantity += 1.0*_credit;
		else
			_intermediate_quantity += 0;
	}
	_GPA = _intermediate_quantity / _all_credit;
	return _GPA;
}
_link* _find_by_string(_link* _head_node, char *_string, int _select_menu)//0id  1name  2school  3major  4class   5 fuzzy id  6 fuzzy name
{
	switch (_select_menu)
	{
	case 0:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._student_id, _string) == 0)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	case 1:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._name, _string) == 0)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	case 2:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._school, _string) == 0)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	case 3:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._major, _string) == 0)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	case 4:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._class, _string) == 0)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	case 5:
		while (_head_node->_next != NULL)
		{
			if (_fuzzy_compare(_head_node->_next->one_student._student_id, _string) == SUCCESS)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	case 6:
		while (_head_node->_next != NULL)
		{
			if (_fuzzy_compare(_head_node->_next->one_student._name, _string) == SUCCESS)
			{
				return _head_node->_next;
			}
			_head_node = _head_node->_next;
		}
		return NULL;
	}
}
int _findout_number(_link* _head_node, char *_string, int _select_menu)//0id  1name  2school  3major  4class   5 fuzzy id  6 fuzzy name
{
	int _found_number = 0;
	switch (_select_menu)
	{
	case 0:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._student_id, _string) == 0)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	case 1:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._name, _string) == 0)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	case 2:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._school, _string) == 0)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	case 3:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._major, _string) == 0)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	case 4:
		while (_head_node->_next != NULL)
		{
			if (strcmp(_head_node->_next->one_student._class, _string) == 0)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	case 5:
		while (_head_node->_next != NULL)
		{
			if (_fuzzy_compare(_head_node->_next->one_student._student_id, _string) == SUCCESS)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	case 6:
		while (_head_node->_next != NULL)
		{
			if (_fuzzy_compare(_head_node->_next->one_student._name, _string) == SUCCESS)
			{
				_found_number++;
			}
			_head_node = _head_node->_next;
		}
		return _found_number;
	}
}
void _load_account(_account_link** _account_head, int* _account_number)
{
	(*_account_head) = (_account_link*)malloc(sizeof(_account_link));
	(*_account_head)->_next = NULL;
	int _error_store = 0;
	char _first_char = '*';
	FILE *_fileptr = NULL;
	_error_store = fopen_s(&_fileptr, _account_file_name, "rb");
	if (_error_store == 0)
	{
		_first_char = fgetc(_fileptr);
		fclose(_fileptr);
	}
	if (_error_store != 0 || _first_char == EOF)
	{
		*_account_number = 0;
	}
	else
	{
		fopen_s(&_fileptr, _account_file_name, "rb");
		_account_link* _node = NULL;
		while (ferror(_fileptr) == 0 && feof(_fileptr) == 0)
		{
			_node = (_account_link*)malloc(sizeof(_account_link));
			fread(&(_node->one_account), sizeof(_account), 1, _fileptr);
			_encrypt_and_decrypt_password_system(_node);
			_node->_next = (*_account_head)->_next;
			(*_account_head)->_next = _node;
			(*_account_number)++;
		}
		fclose(_fileptr);
		if ((*_account_head)->_next != NULL)//为了解决 那个多读取一个的错误 只能让头指针 往后移动一位了
		{
			(*_account_head) = (*_account_head)->_next;
			(*_account_number)--;
		}
	}
}
void _save_account(_account_link* _account_head)
{
	FILE* _fileptr;
	if (fopen_s(&_fileptr, _account_file_name, "wb") != SUCCESS)
	{
		printf("储存失败请重试!"); DELAY_MAX;
		return;
	}
	else
	{
		while (_account_head->_next != NULL)
		{
			_encrypt_and_decrypt_password_system(_account_head->_next);
			fwrite(&(_account_head->_next->one_account), sizeof(_account), 1, _fileptr);
			_encrypt_and_decrypt_password_system(_account_head->_next);
			_account_head = _account_head->_next;
		}
		fclose(_fileptr);
	}
}
void _register_authority(_account_link** _account_head, int* _account_number, int _authority)
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	int _error_store = 0;
	int _pass_store = 0;
	int i = 0;
	char _password[20];
	_account_link* _account_head_copy = (*_account_head);//备份
	if (_authority == ADMIN)
	{
		if (_if_have_ADMIN(*_account_head) == FAIL)
		{

			_register_interface_admin();
			_coordinate.X = 25; _coordinate.Y = 2; SetConsoleCursorPosition(_handle, _coordinate);
		}
		else
		{
			_pass_store = _password_system(_account_head_copy);
			if (_pass_store == SUCCESS)
			{
			REG_ADMIN:  _register_interface_admin();
				AUTHORITY = ADMIN;
				_coordinate.X = 25; _coordinate.Y = 2; SetConsoleCursorPosition(_handle, _coordinate);
			}
			else
			{
				return;
			}
		}
	}
	else
	{
	REG_USER:	_register_interface_user();
		_coordinate.X = 27; _coordinate.Y = 3; SetConsoleCursorPosition(_handle, _coordinate);
	}
	_error_store = 0; i = 0;//每次注册之前初始化
	_account_head_copy = (*_account_head);
	while (_password[i] = _getch(), _password[i] != ENTER)
	{
		if (_password[i] == ESC)
		{
			return;
		}
		if (i == 0 && _password[i] == BACKSPACE)
		{
			RING;
			_password[i] = '\0';
			if (AUTHORITY == ADMIN || _if_have_ADMIN(*_account_head) == FAIL)
				goto REG_ADMIN;
			goto REG_USER;
		}
		else if (_password[i] == BACKSPACE)
		{
		DEL:i--;
			printf("\b \b");
		}
		else
		{
			i++;
			printf("*");
			if (i == 19)
			{
				RING;
				goto DEL;
			}
		}

	}
	if (i == 0 && _password[i] == ENTER)
	{
		RING;
		if (AUTHORITY == ADMIN)
			goto REG_ADMIN;
		else
			goto REG_USER;
	}
	_password[i] = '\0';
	while (_account_head_copy->_next != NULL)
	{
		if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0)
		{
			_error_store = 1;
			_coordinate.X = 12; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			_account_head_copy = (*_account_head);
			printf("密码库中已有相同的密码!"); DELAY_MAX;
			if (AUTHORITY == ADMIN)
				goto REG_ADMIN;
			else
				goto REG_USER;
		}
		else
		{
			_account_head_copy = _account_head_copy->_next;
		}
	}
	if (_error_store == 0)
	{
		_account_link* _account_node = (_account_link*)malloc(sizeof(_account_link));
		strcpy_s(_account_node->one_account._password, 20, _password);
		if (_authority == ADMIN)
			_account_node->one_account._authority = ADMIN;
		else if (_authority == USER)
			_account_node->one_account._authority = USER;
		_account_node->_next = (*_account_head)->_next;
		(*_account_head)->_next = _account_node;
		_coordinate.X = 12; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
		_save_account(*_account_head);
		printf("密码创建成功!"); DELAY_MID;
		(*_account_number)++;
	}
}
void _edit_admin_password(_account_link** _account_head, int* _account_number)
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	int i = 0;
	char _password[20];
	char _select_menu = '*';
EDIT:
	i = 0;
	_account_link* _account_head_copy = (*_account_head);
	if (AUTHORITY == ADMIN)
	{
		_edit_admin_password_interface();
		_coordinate.X = 45; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
		while (_password[i] = _getch(), _password[i] != ENTER)
		{
			if (_password[i] == ESC)
			{
				return;
			}
			if (i == 0 && _password[i] == BACKSPACE)
			{
				RING;
				_password[i] = '\0';
				goto EDIT;
			}
			else if (_password[i] == BACKSPACE)
			{
			DEL:i--;
				printf("\b \b");
			}
			else
			{
				i++;
				printf("*");
				if (i == 19)
				{
					RING;
					goto DEL;
				}
			}

		}
		_password[i] = '\0';

		while (_account_head_copy->_next != NULL)
		{
			if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0 && _account_head_copy->_next->one_account._authority == ADMIN)
			{
				_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
				printf("按D删除该密码ENTER继续!");

				while (1)
				{
					_select_menu = _getch();
					if (_select_menu == 'D' || _select_menu == 'd')
					{
						_account_link* _account_head_copy_free = _account_head_copy->_next;
						_account_head_copy->_next = _account_head_copy->_next->_next;
						free(_account_head_copy_free);
						(*_account_number)--;
						_save_account(*_account_head);
						goto EDIT_INTERFACE;
					}
					else if (_select_menu == ENTER)
					{
					EDIT_B:i = 0;

						_coordinate.X = 45; _coordinate.Y = 7; SetConsoleCursorPosition(_handle, _coordinate);
						while (_password[i] = _getch(), _password[i] != ENTER)
						{
							if (_password[i] == ESC)
							{
								goto EDIT_INTERFACE;
							}
							if (i == 0 && _password[i] == BACKSPACE)
							{
								RING;
								_password[i] = '\0';
								goto EDIT_B;
							}
							else if (_password[i] == BACKSPACE)
							{
							DEL_B:i--;
								printf("\b \b");
							}
							else
							{
								i++;
								printf("*");
								if (i == 19)
								{
									RING;
									goto DEL_B;
								}
							}

						}
						_password[i] = '\0';
						if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0)
						{
							_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
							printf("输入的新密码与原密码相同!"); DELAY_MAX; goto EDIT_B;
						}
						else
						{
							strcpy_s(_account_head_copy->_next->one_account._password, 20, _password);
							_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
							_save_account(*_account_head);
							printf("密码修改成功!"); DELAY_MIN;
							goto EDIT_INTERFACE;
						}
					}
					RING;
				}
			}
			else
			{
				_account_head_copy = _account_head_copy->_next;
			}

		}
		_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
		printf("无该密码!"); DELAY_MAX;
	}

	else
	{
		_coordinate.X = 25; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
		printf("当前权限不足!"); DELAY_MAX;
	EDIT_INTERFACE: return;
	}
}
void _edit_user_password(_account_link** _account_head, int* _account_number)
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	int i = 0;
	char _password[20];
	char _select_menu = '*';
EDIT:
	i = 0;
	_account_link* _account_head_copy = (*_account_head);
	_edit_user_password_interface();
	_coordinate.X = 43; _coordinate.Y = 8; SetConsoleCursorPosition(_handle, _coordinate);
	while (_password[i] = _getch(), _password[i] != ENTER)
	{
		if (_password[i] == ESC)
		{
			return;
		}
		if (i == 0 && _password[i] == BACKSPACE)
		{
			RING;
			_password[i] = '\0';
			goto EDIT;
		}
		else if (_password[i] == BACKSPACE)
		{
		DEL:i--;
			printf("\b \b");
		}
		else
		{
			i++;
			printf("*");
			if (i == 19)
			{
				RING;
				goto DEL;
			}
		}

	}
	_password[i] = '\0';
	while (_account_head_copy->_next != NULL)
	{
		if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0 && _account_head_copy->_next->one_account._authority == USER)
		{
			_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
			printf("按D删除该密码ENTER继续!");
			while (1)
			{
				_select_menu = _getch();
				if (_select_menu == 'D'&&_select_menu == 'd')
				{
					_account_link* _account_head_copy_free = _account_head_copy->_next;
					_account_head_copy->_next = _account_head_copy->_next->_next;
					free(_account_head_copy_free);
					(*_account_number)--;
					_save_account(*_account_head);
					goto EDIT_INTERFACE;
				}
				else if (_select_menu == ENTER)
				{
				EDIT_B:i = 0;
					_coordinate.X = 43; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
					while (_password[i] = _getch(), _password[i] != ENTER)
					{
						if (_password[i] == ESC)
						{
							goto EDIT_INTERFACE;
						}
						if (i == 0 && _password[i] == BACKSPACE)
						{
							RING;
							_password[i] = '\0';
							goto EDIT_B;
						}
						else if (_password[i] == BACKSPACE)
						{
						DEL_B:i--;
							printf("\b \b");
						}
						else
						{
							i++;
							printf("*");
							if (i == 19)
							{
								RING;
								goto DEL_B;
							}
						}

					}
					_password[i] = '\0';
					if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0)
					{
						_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
						printf("输入的新密码与原密码相同"); DELAY_MAX; goto EDIT_B;
					}
					else
					{
						_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
						strcpy_s(_account_head_copy->_next->one_account._password, 20, _password);
						_save_account(*_account_head);
						printf("密码修改成功!"); DELAY_MIN;
						goto EDIT_INTERFACE;
					}
				}
				RING;
			}
		}
		else
		{
			_account_head_copy = _account_head_copy->_next;
		}

	}
	_coordinate.X = 66; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
	printf("无该密码!"); DELAY_MAX;
EDIT_INTERFACE:;
}
void _user_up(_account_link* _account_head)
{
	if (AUTHORITY == ADMIN)
	{
		_user_up_interface();
		int _error_store = 0;
		char _password[20];
		char _select_judge = '*';
		int i = 0;
	UP:i = 0;
		_error_store = 0;
		_coordinate.X = 29; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
		while (_password[i] = _getch(), _password[i] != ENTER)
		{
			if (_password[i] == ESC)
			{
				return;
			}
			if (i == 0 && _password[i] == BACKSPACE)
			{
				RING;
				_password[i] = '\0';
				goto UP;
			}
			else if (_password[i] == BACKSPACE)
			{
			DEL:i--;
				printf("\b \b");
			}
			else
			{
				i++;
				printf("*");
				if (i == 19)
				{
					RING;
					goto DEL;
				}
			}

		}
		if (i == 0 && _password[i] == ENTER)
		{
			RING;
			goto UP;
		}
		_password[i] = '\0';
		while (_account_head->_next != NULL)
		{
			if (strcmp(_account_head->_next->one_account._password, _password) == 0 && _account_head->_next->one_account._authority == USER)
			{
				_error_store = 1;
				_coordinate.X = 50; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
				printf("ENTER键提升权限");
				_select_judge = _getch();
				while (1)
				{
					if (_select_judge == ESC)
					{
						return;
					}
					else if (_select_judge == ENTER)
					{
						_account_head->_next->one_account._authority = ADMIN;
						_save_account(_account_head);
						_coordinate.X = 50; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
						printf("  权限提升成功!"); DELAY_MID; goto MAIN_INTERFACE;
					}

				}
			}
			else
			{
				_account_head = _account_head->_next;
			}
		}
		if (_error_store == 0)
		{
			_coordinate.X = 50; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
			printf("无此用户"); DELAY_MAX;
			return;
		}
	}
	else
	{
		_coordinate.X = 25; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
		printf("当前权限不足!"); DELAY_MAX;
	}
MAIN_INTERFACE:;
}
void _bulk_load(_link** _head_node, int* _number_of_students)
{
	if (AUTHORITY == ADMIN)
	{
		_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(_handle, &_screen_info);
		int _option = 0;
		int _error_store = 0;
		int _number = 0;//用于储存token 读取的数字
		char* token = NULL;
		char* next_token = NULL;
		char _first_char = '*';
		char string[sizeof(_student) + 15 * sizeof(char)];
		_link* _node = NULL;
		strcpy_s(string, sizeof("姓名,学号,性别1男2女,学院,专业,班级,生日 年,月,日,高等数学,程序设计,数据结构,大学英语,大学物理,体育,选修课,\n,,,,,,,,,,,,,,,,#,#放在末尾不能删除"), "姓名,学号,性别1男2女,学院,专业,班级,生日 年,月,日,高等数学,程序设计,数据结构,大学英语,大学物理,体育,选修课,\n,,,,,,,,,,,,,,,,#,#放在末尾不能删除");
		FILE* _fileptr;
		_error_store = fopen_s(&_fileptr, _bluk_import_file_name, "rt");
		if (_error_store != SUCCESS)//判断文件是否存在，如果不存在则创建，如果存在则不创建
		{
			fopen_s(&_fileptr, _bluk_import_file_name, "wt");
			fwrite(string, sizeof("姓名,学号,性别1男2女,学院,专业,班级,生日 年,月,日,高等数学,程序设计,数据结构,大学英语,大学物理,体育,选修课,\n,,,,,,,,,,,,,,,,#,#放在末尾不能删除"), 1, _fileptr);
			_coordinate.X = 25; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
			fclose(_fileptr);
			printf("请找到文件:student_information_bluk_import.csv(初始位置D盘根目录)");
			_coordinate.X = 25; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
			printf("用Excel打开输入信息"); _getch();
		}
		else
		{
			fseek(_fileptr, 110L, 0);
			_first_char = fgetc(_fileptr);
			rewind(_fileptr);
			fseek(_fileptr, 110L, 0);
			if (_first_char == ',')//判断是否为空
			{
				_coordinate.X = 25; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
				fclose(_fileptr);
				printf("请找到文件:student_information_bluk_import.csv(初始位置D盘根目录)");
				_coordinate.X = 25; _coordinate.Y = 11; SetConsoleCursorPosition(_handle, _coordinate);
				printf("用Excel打开输入信息"); _getch();
			}
			else
			{
				while (fgets(string, sizeof(_student) + 17 * sizeof(char), _fileptr) != NULL)
				{
					_option = 0;
					_node = (_link*)malloc(sizeof(_link));
					token = strtok_s(string, ",", &next_token);
					while (token != NULL &&strcmp(token, "\n") != 0)
					{
						switch (_option)
						{
						case 0:     strcpy_s(_node->one_student._name, 20 * sizeof(char), token); break;
						case 1: 	strcpy_s(_node->one_student._student_id, 16 * sizeof(char), token); break;
						case 2:     sscanf_s(token, "%d", &_number); _node->one_student._sex = _number; break;
						case 3: 	strcpy_s(_node->one_student._school, 40 * sizeof(char), token); break;
						case 4:  	strcpy_s(_node->one_student._major, 40 * sizeof(char), token); break;
						case 5:	    strcpy_s(_node->one_student._class, 14 * sizeof(char), token); break;
						case 6:     sscanf_s(token, "%d", &_number); _node->one_student._birthday._year = _number; break;
						case 7:     sscanf_s(token, "%d", &_number); _node->one_student._birthday._month = _number; break;
						case 8:     sscanf_s(token, "%d", &_number); _node->one_student._birthday._day = _number; break;
						case 9:     sscanf_s(token, "%d", &_number); _node->one_student._score._advanced_mathematics = _number; break;
						case 10:    sscanf_s(token, "%d", &_number); _node->one_student._score._langue_programming = _number; break;
						case 11:    sscanf_s(token, "%d", &_number); _node->one_student._score._data_struct = _number; break;
						case 12:    sscanf_s(token, "%d", &_number); _node->one_student._score._college_English = _number; break;
						case 13:    sscanf_s(token, "%d", &_number); _node->one_student._score._college_physics = _number; break;
						case 14:    sscanf_s(token, "%d", &_number); _node->one_student._score._physical_education = _number; break;
						case 15:    sscanf_s(token, "%d", &_number); _node->one_student._score._selective_course = _number; break;
						}
						token = strtok_s(NULL, ",", &next_token);
						_option++;
					}
					_node->one_student._score._GPA = _caculate_gpa(_node->one_student._score._advanced_mathematics, _node->one_student._score._langue_programming, _node->one_student._score._data_struct, _node->one_student._score._college_English, _node->one_student._score._physical_education, _node->one_student._score._physical_education, _node->one_student._score._selective_course);
					_node->_next = (*_head_node)->_next;
					(*_head_node)->_next = _node;
					(*_number_of_students)++;
				}
				_coordinate.X = 25; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
				_save_student_information(*_head_node);
			}
		}
	}
	else
	{
		_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
		printf("当前权限不足!"); DELAY_MAX;
	}
}
int _if_have_ADMIN(_account_link *_account_head)
{
	while (_account_head->_next != NULL)
	{
		if (_account_head->_next->one_account._authority == ADMIN)
			return SUCCESS;
		_account_head = _account_head->_next;
	}
	return FAIL;
}
void _sort_student_information(_link* _head_node)
{
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	double a, b;
	_link * _copy = NULL;
	_student _istudent;
	while (_head_node->_next != NULL)
	{
		_copy = _head_node->_next;
		while (_copy->_next != NULL)
		{
			if (sscanf_s(_head_node->_next->one_student._student_id, "%lf", &a), sscanf_s(_copy->_next->one_student._student_id, "%lf", &b), a > b)
			{
				_istudent = _head_node->_next->one_student;
				_head_node->_next->one_student = _copy->_next->one_student;
				_copy->_next->one_student = _istudent;
			}
			_copy = _copy->_next;
		}
		_head_node = _head_node->_next;
	}
	_coordinate.X = 25; _coordinate.Y = 9; SetConsoleCursorPosition(_handle, _coordinate);
	printf("排序成功!"); DELAY_MAX;
}
void _setting()
{
	char  _file_copy[200];
	_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(_handle, &_screen_info);
	char _file_name[200];
	char _select_menu = '*';
	_setting_interface();
	_select_menu = _getch();
	if (_select_menu == ESC)
	{
		return;
	}
	else
	{
		_select_menu -= '0';
		while (1)
		{
			switch (_select_menu)
			{
			case 1:
			{
				_coordinate.X = 30; _coordinate.Y = 2; SetConsoleCursorPosition(_handle, _coordinate);
				scanf_s("%s", _file_name, 200);
				strcpy_s(_file_copy, 200, _student_file_name);
				strcpy_s(_student_file_name, 200, _file_name);
				_setting_save();
				_add_slash(_file_name);
				MoveFile(_file_copy, _file_name);

			}break;
			case 2:
			{
				_coordinate.X = 30; _coordinate.Y = 4; SetConsoleCursorPosition(_handle, _coordinate);
				scanf_s("%s", _file_name, 200);
				strcpy_s(_file_copy, 200, _account_file_name);
				strcpy_s(_account_file_name, 200, _file_name);
				_setting_save();
				_add_slash(_file_name);
				MoveFile(_file_copy, _file_name);
			}break;
			case 3:
			{
				_coordinate.X = 30; _coordinate.Y = 6; SetConsoleCursorPosition(_handle, _coordinate);
				scanf_s("%s", _file_name, 200);
				strcpy_s(_file_copy, 200, _bluk_import_file_name);
				strcpy_s(_bluk_import_file_name, 200, _file_name);
				_setting_save();
				_add_slash(_file_name);
				MoveFile(_file_copy, _file_name);
			}break;
			case 4:
			{
				_coordinate.X = 28; _coordinate.Y = 10; SetConsoleCursorPosition(_handle, _coordinate);
				printf("按下回车键确认清空所有文件");
				char ch;
				while (1)
				{
					ch = _getch();
					if (ch == ENTER)
					{
						DeleteFile(_student_file_name);
						DeleteFile(_account_file_name);
						DeleteFile(_bluk_import_file_name);
						printf("成功"); break;
					}
					else if (ch == ESC)
					{
						break;
					} 
				}
			}break;
			default:RING;
			}
			_setting_interface();
			_select_menu = _getch();
			if (_select_menu == ESC)
			{
				return;
			}
			_select_menu -= '0';
		}
	}

}
void _setting_initialize()
{
	char string[200];
	int _option = 0;
	char* token = NULL;
	char* next_token = NULL;
	int _error_store = 0;
	char _first_char = '*';
	FILE *_fileptr = NULL;
	_error_store = fopen_s(&_fileptr, "d:\\config.ini", "rb");
	if (_error_store == 0)
	{
		_first_char = fgetc(_fileptr);
		fclose(_fileptr);
	}
	if (_error_store != 0 || _first_char == EOF)
	{
		fopen_s(&_fileptr, "d:\\config.ini", "wb");
		strcpy_s(_student_file_name, 200, "D:\\student_information.dat");
		strcpy_s(_account_file_name, 200, "D:\\account_information.dat");
		strcpy_s(_bluk_import_file_name, 200, "D:\\student_information_bluk_import.csv");
		fprintf(_fileptr, "%s", _student_file_name); fprintf(_fileptr, "%c", ',');
		fprintf(_fileptr, "%s", _account_file_name); fprintf(_fileptr, "%c", ',');
		fprintf(_fileptr, "%s", _bluk_import_file_name); fprintf(_fileptr, "%c", ',');
		fclose(_fileptr);
	}
	else
	{
		fopen_s(&_fileptr, "d:\\config.ini", "rb");
		fgets(string, sizeof(char) * 700, _fileptr);
		_option = 0;
		token = strtok_s(string, ",", &next_token);
		while (_option!=3)
		{
			switch (_option)
			{
			case 0:     strcpy_s(_student_file_name, 200 * sizeof(char), token); break;
			case 1: 	strcpy_s(_account_file_name, 200 * sizeof(char), token); break;
			case 2:     strcpy_s(_bluk_import_file_name, 200 * sizeof(char), token); break;
			}
			token = strtok_s(NULL, ",", &next_token);
			_option++;
		}
		fclose(_fileptr);
	}
}
void _add_slash(char *_name)
{
	int i = 0, j = 0;
	char ini1 = '*', ini2 = '*';
	while (_name[i] != '\0') 
	{
		if (_name[i] == '\\')
		{
			j = i;
			ini1 = _name[j];
			while (ini1 != '\0')
			{
				ini2 = ini1;
				ini1 = _name[j + 1];
				_name[j + 1] = ini2;
				j++;
			}
			_name[j+1] = '\0';
			i++;
		}
		i++;
	}
}
void _setting_save()
{
	FILE *_fileptr = NULL;
	fopen_s(&_fileptr,"d:\\config.ini", "wb");
	fprintf(_fileptr, "%s", _student_file_name); fprintf(_fileptr, "%c", ',');
	fprintf(_fileptr, "%s", _account_file_name); fprintf(_fileptr, "%c", ',');
	fprintf(_fileptr, "%s", _bluk_import_file_name); fprintf(_fileptr, "%c", ',');
	fclose(_fileptr);
}
