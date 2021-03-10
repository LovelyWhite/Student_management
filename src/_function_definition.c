#include "_program_all_header.h"
extern char _student_file_name[200];
extern char _start_password[20];
extern char _account_file_name[200];
extern char _bluk_import_file_name[200];
extern int AUTHORITY;
int fopen_s(FILE **_fileptr, char *file_name, char *mode)
{
	*_fileptr = fopen(file_name, mode);
	if (*_fileptr != NULL)
	{
		return 0;
	}
	return -1;
}
void strcpy_s(char *dst, int size, char *src)
{
	strcpy(dst, src);
}

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &old);			/* grab old terminal i/o settings */
	current = old;				/* make new settings same as old settings */
	current.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo)
	{
		current.c_lflag |= ECHO; /* set echo mode */
	}
	else
	{
		current.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void)
{
	return getch_(0);
}

void _load_student_information(_link **_head_node, int *_number_of_students) // 用二级指针的目的是为了改变该指针的值
{
	(*_head_node) = (_link *)malloc(sizeof(_link));
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
		_link *_node = NULL;
		while (ferror(_fileptr) == 0 && feof(_fileptr) == 0)
		{
			_node = (_link *)malloc(sizeof(_link));
			fread(&(_node->one_student), sizeof(_student), 1, _fileptr);
			_node->_next = (*_head_node)->_next;
			(*_head_node)->_next = _node;
			(*_number_of_students)++;
		}
		fclose(_fileptr);
		if ((*_head_node)->_next != NULL) // 为了解决 那个多读取一个的错误 只能让头指针 往后移动一位了
		{
			(*_head_node) = (*_head_node)->_next;
			(*_number_of_students)--;
		}
	}
}
void _find_student_information(_link *_head_node)
{
	char _select_menu = '*';
	_find_interface();
	_select_menu = getch();
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
			case 1:
				_find_by_student_id(_head_node);
				break;
			case 2:
				_find_by_name(_head_node);
				break;
			case 3:
				_find_by_school(_head_node);
				break;
			case 4:
				_find_by_major(_head_node);
				break;
			case 5:
				_find_by_class(_head_node);
				break;
			case 6:
				_fuzzy_search(_head_node);
				break;
			default:;
			}
			_find_interface();
			_select_menu = getch();
			if (_select_menu == ESC)
			{
				goto MAIN_INTERFACE;
			}
			_select_menu -= '0';
		}
	}
}
void _add_student_information(_link **_head_node, int *_number_of_students)
{
	char _select_judge = 'Y';
	if (AUTHORITY == ADMIN)
	{
		_link *_node = (_link *)malloc(sizeof(_link));
		while (_select_judge == 'Y' || _select_judge == 'y')
		{
			_main_interface();
			_input_record(_node, *_number_of_students, *_head_node);
			_node->_next = (*_head_node)->_next;
			(*_head_node)->_next = _node;
			_main_interface();
			_move_cursor(5, 10);
			_save_student_information(*_head_node);
			(*_number_of_students)++;
			while (1)
			{
				_main_interface();
				_move_cursor(5, 10);
				printf("是否继续添加（Y/N）");
				_select_judge = getch();
				if (_select_judge == 'N' || _select_judge == 'n')
					goto MAIN_INTERFACE;
				else if (_select_judge == 'Y' || _select_judge == 'y')
					break;
			}
		}
	}
	else
	{
		_move_cursor(5, 9);
		printf("当前权限不足!");
		DELAY_MAX;
	MAIN_INTERFACE:;
	}
}
void _del_student_information(_link *_head_node, int *_number_of_students)
{
	char _id[20];
	char _select_judge = 'N';
	if (AUTHORITY == ADMIN)
	{
		_del_interface();
		_link *_found = _head_node, *_copy = NULL;
		_move_cursor(0, 6);
		scanf("%s", _id);
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
				_select_judge = getch();
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
			_move_cursor(0, 6);
			printf("无此学生信息!");
			DELAY_MAX;
		}
	}
	else
	{
		_move_cursor(5, 9);
		printf("当前权限不足!");
		DELAY_MAX;
	}
}
void _edit_student_information(_link *_head_node)
{
	if (AUTHORITY == ADMIN)
	{
		char _key = '*';
		char _strcmp_sex[3];
		char _id[20];
		_link *_findout = NULL;
		_move_cursor(8, 7);
		printf(":请输入学生的学号(不多于18位):");
		scanf("%s", _id);
		_findout = _find_by_string(_head_node, _id, 0);
		if (_findout != NULL)
		{
			_putout_student_information(_findout, 1, 1);
			_move_cursor(4, 12);
			printf("按E键编辑");
			while (1)
			{
				_key = getch();
				if (_key == ESC)
				{
					goto MAIN_INTERFACE;
				}
				else if (_key == ENTER)
				{
					goto start;
				}
			}
		start:
			_move_cursor(1, 3);
			_key = getch();
			if (_key == ENTER)
			{
				goto a;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf("%s", _findout->one_student._name);
			}
		a:
			_move_cursor(1, 4);
			_key = getch();
			if (_key == ENTER)
			{
				goto SEX;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf("%s", _findout->one_student._student_id);
			}
		SEX:
			_move_cursor(1, 5);
			_key = getch();
			if (_key == ENTER)
			{
				goto b;
			}
			else
			{
			_SEX:
				printf("        \b\b\b\b\b\b\b\b");
				scanf("%s", _strcmp_sex);
			}
			if (strcmp("男", _strcmp_sex) == 0)
				_head_node->one_student._sex = 1;
			else if (strcmp("女", _strcmp_sex) == 0)
				_head_node->one_student._sex = 0;
			else
				goto _SEX;
		b:
			_move_cursor(1, 6);
			_key = getch();
			if (_key == ENTER)
			{
				goto c;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf("%s", _findout->one_student._school);
			}
		c:
			_move_cursor(1, 7);
			_key = getch();
			if (_key == ENTER)
			{
				goto d;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf("%s", _findout->one_student._major);
			}
		d:
			_move_cursor(1, 8);
			_key = getch();
			if (_key == ENTER)
			{
				goto e;
			}
			else
			{
				printf("        \b\b\b\b\b\b\b\b");
				scanf("%s", _findout->one_student._class);
			}
		e:
			_move_cursor(1, 9);
			_key = getch();
			if (_key == ENTER)
			{
				goto f;
			}
			else
			{
				printf("    \b\b\b\b");
				scanf("%d", &_findout->one_student._birthday._year);
			}
		f:
			_move_cursor(7, 9);
			_key = getch();
			if (_key == ENTER)
			{
				goto g;
			}
			else
			{
				printf("  \b\b");
				scanf("%d", &_findout->one_student._birthday._month);
			}
		g:
			_move_cursor(1, 9);
			_key = getch();
			if (_key == ENTER)
			{
				goto h;
			}
			else
			{
				printf("  \b\b");
				scanf("%d", &_findout->one_student._birthday._day);
			}
		h:
			_move_cursor(6, 3);
			_key = getch();
			if (_key == ENTER)
			{
				goto i;
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._advanced_mathematics);
			}
		i:
			_move_cursor(6, 4);
			_key = getch();
			if (_key == ENTER)
			{
				goto j;
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._langue_programming);
			}
		j:
			_move_cursor(6, 5);
			_key = getch();
			if (_key == ENTER)
			{
				goto k;
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._data_struct);
			}
		k:
			_move_cursor(6, 6);
			_key = getch();
			if (_key == ENTER)
			{
				goto l;
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._college_English);
			}
		l:
			_move_cursor(6, 7);
			_key = getch();
			if (_key == ENTER)
			{
				goto m;
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._college_physics);
			}
		m:
			_move_cursor(6, 8);
			_key = getch();
			if (_key == ENTER)
			{
				goto n;
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._physical_education);
			}
		n:
			_move_cursor(6, 9);
			_key = getch();
			if (_key == ENTER)
			{
			}
			else
			{
				printf("   \b\b\b");
				scanf("%d", &_findout->one_student._score._selective_course);
			}
			_head_node->one_student._score._GPA = _caculate_gpa(_findout->one_student._score._advanced_mathematics, _findout->one_student._score._langue_programming, _findout->one_student._score._data_struct, _findout->one_student._score._college_English, _findout->one_student._score._physical_education, _findout->one_student._score._physical_education, _findout->one_student._score._selective_course);
			_main_interface();
			_move_cursor(0, 10);
			_save_student_information(_head_node);
		}
		else
		{
			_move_cursor(5, 9);
			printf("未找到该学号学生!");
			DELAY_MAX;
		MAIN_INTERFACE:;
		}
	}
	else
	{
		_move_cursor(5, 9);
		printf("当前权限不足!");
		DELAY_MAX;
	}
}
void _putout_all_student_information(_link *_head_node, int _number_of_students)
{
	char _first_char = '*';
	int _error_store = 0;
	FILE *_fileptr = NULL;
	_error_store = fopen_s(&_fileptr, _student_file_name, "rb");
	if (_error_store == 0)
	{
		_first_char = fgetc(_fileptr);
		fclose(_fileptr);
	}
	if (_error_store != 0 || _first_char == EOF)
	{
		_move_cursor(5, 9);
		printf("无学生信息，请添加!");
		DELAY_MAX;
	MAIN_INTERFACE:;
	}
	else
	{
		char _judge = '*';
		int i = 1;
		while (_head_node->_next != NULL)
		{
			_putout_student_information(_head_node->_next, _number_of_students, i);
			i++;
			_judge = getch();
			while (1)
			{
				if (_judge == ENTER)
				{
					_head_node = _head_node->_next;
					break;
				}
				else if (_judge == ESC)
					goto MAIN_INTERFACE; // 退出循环
			}
		}
	}
}
void _edit_password(_account_link **_account_head, int *_account_number)
{
	char _select_menu = '*';
	_edit_password_interface();
	_select_menu = getch();
	if (_select_menu == ESC)
	{
	MAIN_INTERFACE:
		return;
	}
	_select_menu -= '0';
	while (1)
	{
		switch (_select_menu)
		{
		case 1:
			_edit_admin_password(_account_head, _account_number);
			break;
		case 2:
			_edit_user_password(_account_head, _account_number);
			break;
		default:;
		}
		_edit_password_interface();
		_select_menu = getch();
		if (_select_menu == ESC)
		{
			goto MAIN_INTERFACE;
		}
		_select_menu -= '0';
	}
}
int _password_system(_account_link *_account_head) // 密码正确返回 SUCCESS 否则返回 FAIL
{
	char _password_judge[20];
	int i = 0;
LOGIN:
	_password_system_interface();
	_move_cursor(6, 5);
	while (_password_judge[i] = getch(), _password_judge[i] != ENTER)
	{
		if (_password_judge[i] == ESC)
			return EXIT;
		if (i == 0 && _password_judge[i] == BACKSPACE)
		{
			_password_judge[i] = '\0';
			goto LOGIN;
		}
		else if (_password_judge[i] == BACKSPACE)
		{
		DEL:
			i--;
			printf("\b \b");
		}
		else
		{
			i++;
			printf("*");
			if (i == 19)
			{
				goto DEL;
			}
		}
	}
	if (i == 0 && _password_judge[i] == ENTER) // 放在while 循环外的原因是while循环是 以enter结束的
	{
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
				_move_cursor(8, 6);
				printf("权限不足!");
				DELAY_MAX;
				return FAIL;
			}
			else
			{
				_move_cursor(8, 6);
				printf("欢迎您管理员!");
				DELAY_MID;
				return SUCCESS;
			}
		}
	}
	_move_cursor(8, 6);
	printf("密码错误请重试!");
	DELAY_MAX;
	return FAIL;
}
int _log_in(_account_link *_account_head, int _account_number)
{
	_account_link *_account_head_copy = _account_head;
	int _errtime = 0;
	char _password_judge[20];
	int i = 0;
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
			   "|__________________________________________________________________________________________|\n");
		;
		DELAY_MAX;
		return EXIT;
	}
	else
	{
	LOGIN:
		CLS_SCREEN;
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
			   "|__________________________________________________________________________________________|\n",
			   3 - _errtime);
		_move_cursor(5, 3);
		i = 0;
		while (_password_judge[i] = getch(), _password_judge[i] != ENTER)
		{
			if (_password_judge[i] == ESC)
			{
				return EXIT;
			}
			if (i == 0 && _password_judge[i] == BACKSPACE)
			{
				_password_judge[i] = '\0';
				goto LOGIN;
			}
			else if (_password_judge[i] == BACKSPACE)
			{
			DEL:
				i--;
				printf("\b \b");
			}
			else
			{
				i++;
				printf("*");
				if (i == 19)
				{
					goto DEL;
				}
			}
		}
		if (i == 0 && _password_judge[i] == ENTER)
		{
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
				_move_cursor(4, 6);
				printf("欢迎使用!");
				DELAY_MID;
				return SUCCESS;
			}
		}
		if (_errtime == 2)
		{
			_move_cursor(4, 6);
			printf("尝试次数超过三次，系统即将退出!");
			DELAY_MAX;
			DELAY_MAX;
			DELAY_MAX;
			DELAY_MAX;
			return FAIL;
		}
		else
		{
			_move_cursor(4, 6);
			printf("密码错误请重试!");
			DELAY_MAX;
			_account_head_copy = _account_head;
			_errtime++;
			goto LOGIN;
		}
	}
}
void _register(_account_link **_account_head, int *_account_number)
{
	char _select_menu = '*';
	if (_if_have_ADMIN(*_account_head) == FAIL)
	{
		_move_cursor(5, 2);
		printf("请先创建一个管理员密码!");
		DELAY_MAX;
		goto REG;
	}
	else
	{
		_register_interface();
		_select_menu = getch();
		if (_select_menu == ESC)
		{
			return;
		}
		else
		{
			_select_menu -= '0';
			while (1)
			{
				AUTHORITY = USER; // 初始化权限
				switch (_select_menu)
				{
				REG:
				case 1:
					_register_authority(_account_head, _account_number, ADMIN);
					break;
				case 2:
					if (_if_have_ADMIN(*_account_head) == FAIL) // 用于防止一个系统bug
					{
						_move_cursor(5, 6);
						printf("请先创建一个管理员密码!");
						DELAY_MAX;
						goto REG;
					}
					else
						_register_authority(_account_head, _account_number, USER);
					break;
				default:;
				}
				_register_interface();
				_select_menu = getch();
				if (_select_menu == ESC)
					return;
				else
					_select_menu -= '0';
			}
		}
	}
}
int _start_function(_account_link **_account_head, int *_account_number)
{
	int _judge = 0;
	char _select_menu = '*';
	_start_interface();
	_select_menu = getch();
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
			case 1:
				_register(_account_head, _account_number);
				break;
			case 2:
				_judge = _log_in(*_account_head, *_account_number);
				if (_judge == SUCCESS)
				{
					return SUCCESS;
				}
				else if (_judge == FAIL)
				{
					return FAIL;
				}
				else // EXIT 这个else是回到主界面
				{
					break;
				}
			default:;
			}
			_start_interface();
			_select_menu = getch();
			if (_select_menu == ESC)
			{
				return EXIT;
			}
			_select_menu -= '0';
		}
	}
}
void _input_record(_link *_node, int _number_of_students, _link *_head_node)
{
	char _strcmp_sex[3];
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
		   "|__________________________________________________________________________________________|\n",
		   _number_of_students + 1);
	_move_cursor(0, 3);
	scanf("%s", _node->one_student._name);
ID:
	_move_cursor(0, 4);
	scanf("%s", _node->one_student._student_id);
	if (0 < _findout_number(_head_node, _node->one_student._student_id, 0))
	{
		goto ID;
	}
SEX:
	_move_cursor(0, 5);
	scanf("%s", _strcmp_sex);
	if (strcmp("男", _strcmp_sex) == 0)
		_node->one_student._sex = 1;
	else if (strcmp("女", _strcmp_sex) == 0)
		_node->one_student._sex = 0;
	else
	{
		goto SEX;
	}
	_move_cursor(0, 6);
	scanf("%s", _node->one_student._school);
	_move_cursor(0, 7);
	scanf("%s", _node->one_student._major);
	_move_cursor(0, 8);
	scanf("%s", _node->one_student._class);
	_move_cursor(1, 9);
	scanf("%d", &_node->one_student._birthday._year);
	_move_cursor(7, 9);
	scanf("%d", &_node->one_student._birthday._month);
	_move_cursor(1, 9);
	scanf("%d", &_node->one_student._birthday._day);
	_move_cursor(6, 3);
	scanf("%d", &_node->one_student._score._advanced_mathematics);
	_move_cursor(6, 4);
	scanf("%d", &_node->one_student._score._langue_programming);
	_move_cursor(6, 5);
	scanf("%d", &_node->one_student._score._data_struct);
	_move_cursor(6, 6);
	scanf("%d", &_node->one_student._score._college_English);
	_move_cursor(6, 7);
	scanf("%d", &_node->one_student._score._college_physics);
	_move_cursor(6, 8);
	scanf("%d", &_node->one_student._score._physical_education);
	_move_cursor(6, 9);
	scanf("%d", &_node->one_student._score._selective_course);
	_node->one_student._score._GPA = _caculate_gpa(_node->one_student._score._advanced_mathematics, _node->one_student._score._langue_programming, _node->one_student._score._data_struct, _node->one_student._score._college_English, _node->one_student._score._physical_education, _node->one_student._score._physical_education, _node->one_student._score._selective_course);
}
void _save_student_information(_link *_head_node)
{
	FILE *_fileptr;
	if (fopen_s(&_fileptr, _student_file_name, "wb") != 0)
	{
		printf("储存失败请重试!");
		DELAY_MAX;
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
		printf("保存成功!");
		DELAY_MID;
	}
}
int _fuzzy_compare(char *_origin, char *_textin)
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
			_err = 0; // 放在这里的原因是为了不影响下一次推移的比较
			for (j = 0; j < _count_textin; j++)
			{
				if (_textin[j] == _origin[j + i])
				{
					_err++;
				}
			}
			if (_err == _count_textin) // 一比较完就判断是否相同
			{
				return SUCCESS;
			}
			else
				return FAIL;
		}
	}
}
void _find_by_student_id(_link *_head_node)
{
	int _now_number = 0;
	_link *_findout = NULL;
	int _findout_sign = FAIL;
	char _id[20], _judge = '*';
	_find_interface_id();
	_move_cursor(8, 4);
	scanf("%s", _id);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _id, 0);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, 1, 1);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(3, 5);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _find_by_name(_link *_head_node)
{
	int _found_number = 0, _now_number = 0;
	_link *_findout;
	int _findout_sign = FAIL;
	char _name[40], _judge = '*';
	_find_interface_name();
	_move_cursor(8, 5);
	scanf("%s", _name);
	_found_number = _findout_number(_head_node, _name, 1);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _name, 1);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(3, 5);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
} // 输入问题
void _find_by_school(_link *_head_node)
{
	int _found_number = 0, _now_number = 0;
	_link *_findout;
	int _findout_sign = FAIL;
	char _school[40], _judge = '*';
	_find_interface_school();
	_move_cursor(8, 6);
	scanf("%s", _school);
	_found_number = _findout_number(_head_node, _school, 2);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _school, 2);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(3, 5);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _find_by_major(_link *_head_node)
{
	int _found_number = 0, _now_number = 0;
	_link *_findout;
	int _findout_sign = FAIL;
	char _major[40], _judge = '*';
	_find_interface_major();
	_move_cursor(8, 7);
	scanf("%s", _major);
	_found_number = _findout_number(_head_node, _major, 3);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _major, 3);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(3, 5);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _find_by_class(_link *_head_node)
{
	int _found_number = 0, _now_number = 0;
	_link *_findout;
	int _findout_sign = FAIL;
	char _class[40], _judge = '*';
	_find_interface_class();
	_move_cursor(8, 8);
	scanf("%s", _class);
	_found_number = _findout_number(_head_node, _class, 4);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _class, 4);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(3, 5);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _fuzzy_search(_link *_head_node)
{
	char _select_menu = '*';
	_fuzzy_search_interface();
	_select_menu = getch();
	if (_select_menu == ESC)
	{
	FIND_INTERFACE:
		_find_interface();
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
				_fuzzy_search_by_name(_head_node);
				_fuzzy_search_interface();
				break;
			case 2:
				_fuzzy_search_by_student_id(_head_node);
				_fuzzy_search_interface();
				break;
			default:;
			}
			_select_menu = getch();
			if (_select_menu == ESC)
			{
				_find_interface();
				goto FIND_INTERFACE;
			}
			_select_menu -= '0';
		}
	}
}
void _fuzzy_search_by_name(_link *_head_node)
{
	int _found_number = 0, _now_number = 0;
	_link *_findout;
	int _findout_sign = FAIL;
	char _name[20], _judge = '*';
	_fuzzy_search_interface_name();
	_move_cursor(8, 7);
	scanf("%s", _name);
	_found_number = _findout_number(_head_node, _name, 6);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _name, 6);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(2, 8);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _fuzzy_search_by_student_id(_link *_head_node)
{
	int _found_number = 0, _now_number = 0;
	_link *_findout;
	int _findout_sign = FAIL;
	char _id[20], _judge = '*';
	_fuzzy_search_interface_id();
	_move_cursor(8, 8);
	scanf("%s", _id);
	_found_number = _findout_number(_head_node, _id, 5);
	while (_head_node->_next != NULL)
	{
		_findout = _find_by_string(_head_node, _id, 5);
		if (_findout != NULL)
		{
			_now_number++;
			_putout_student_information(_findout, _found_number, _now_number);
			_judge = getch();
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
				_judge = getch();
			}
			_findout_sign = SUCCESS;
		}
		_head_node = _head_node->_next;
	}
	if (_findout_sign == FAIL)
	{
		_move_cursor(2, 8);
		printf("未找到该学生");
		DELAY_MAX;
	}
FIND_INTERFACE:;
}
void _encrypt_and_decrypt_password_system(_account_link *_account_head)
{
	int i = 0;
	char key[] = {'a', '4', 'c', 'E', 'b', 'C', 'K', 'w', 'V', 'b', 'Y', 'n', 'J', 'e', 'Y', 'j', 'l', '0', 'v'};
	for (i = 0; i < 20; i++)
		(_account_head->one_account._password[i]) ^= key[i];
}
void _putout_student_information(_link *_node, int _findout_number, int _now_number)
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
		   "|__________________________________________________________________________________________|\n",
		   _node->one_student._name, _node->one_student._score._advanced_mathematics, _node->one_student._student_id, _node->one_student._score._langue_programming, __sex, _node->one_student._score._data_struct, _node->one_student._school, _node->one_student._score._college_English, _node->one_student._major, _node->one_student._score._college_physics, _node->one_student._class, _node->one_student._score._physical_education, _node->one_student._birthday._year, _node->one_student._birthday._month, _node->one_student._birthday._day, _node->one_student._score._selective_course, _node->one_student._score._GPA, _now_number, _findout_number);
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
		case 0:
			_score = _advanced_mathematics;
			_credit = credit_advanced_mathematics;
			break;
		case 1:
			_score = _langue_programming;
			_credit = credit_langue_programming;
			break;
		case 2:
			_score = _data_struct;
			_credit = credit_data_struct;
			break;
		case 3:
			_score = _college_English;
			_credit = credit_college_English;
			break;
		case 4:
			_score = _college_physics;
			_credit = credit_college_physics;
			break;
		case 5:
			_score = _physical_education;
			_credit = credit_physical_education;
			break;
		case 6:
			_score = _selective_course;
			_credit = credit_selective_course;
			break;
		}
		if (_score >= 90)
			_intermediate_quantity += 4.0 * _credit;
		else if (_score >= 80)
			_intermediate_quantity += 3.0 * _credit;
		else if (_score >= 70)
			_intermediate_quantity += 2.0 * _credit;
		else if (_score >= 60)
			_intermediate_quantity += 1.0 * _credit;
		else
			_intermediate_quantity += 0;
	}
	_GPA = _intermediate_quantity / _all_credit;
	return _GPA;
}
_link *_find_by_string(_link *_head_node, char *_string, int _select_menu) // 0id  1name  2school  3major  4class   5 fuzzy id  6 fuzzy name
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
int _findout_number(_link *_head_node, char *_string, int _select_menu) // 0id  1name  2school  3major  4class   5 fuzzy id  6 fuzzy name
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
void _load_account(_account_link **_account_head, int *_account_number)
{
	(*_account_head) = (_account_link *)malloc(sizeof(_account_link));
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
		_account_link *_node = NULL;
		while (ferror(_fileptr) == 0 && feof(_fileptr) == 0)
		{
			_node = (_account_link *)malloc(sizeof(_account_link));
			fread(&(_node->one_account), sizeof(_account), 1, _fileptr);
			_encrypt_and_decrypt_password_system(_node);
			_node->_next = (*_account_head)->_next;
			(*_account_head)->_next = _node;
			(*_account_number)++;
		}
		fclose(_fileptr);
		if ((*_account_head)->_next != NULL) // 为了解决 那个多读取一个的错误 只能让头指针 往后移动一位了
		{
			(*_account_head) = (*_account_head)->_next;
			(*_account_number)--;
		}
	}
}
void _save_account(_account_link *_account_head)
{
	FILE *_fileptr;
	if (fopen_s(&_fileptr, _account_file_name, "wb") != SUCCESS)
	{
		printf("储存失败请重试!");
		DELAY_MAX;
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
void _register_authority(_account_link **_account_head, int *_account_number, int _authority)
{
	int _error_store = 0;
	int _pass_store = 0;
	int i = 0;
	char _password[20];
	_account_link *_account_head_copy = (*_account_head); // 备份
	if (_authority == ADMIN)
	{
		if (_if_have_ADMIN(*_account_head) == FAIL)
		{
			_register_interface_admin();
			_move_cursor(5, 2);
		}
		else
		{
			_pass_store = _password_system(_account_head_copy);
			if (_pass_store == SUCCESS)
			{
			REG_ADMIN:
				_register_interface_admin();
				AUTHORITY = ADMIN;
				_move_cursor(5, 2);
			}
			else
			{
				return;
			}
		}
	}
	else
	{
	REG_USER:
		_register_interface_user();
		_move_cursor(7, 3);
	}
	_error_store = 0;
	i = 0; // 每次注册之前初始化
	_account_head_copy = (*_account_head);
	while (_password[i] = getch(), _password[i] != ENTER)
	{
		if (_password[i] == ESC)
		{
			return;
		}
		if (i == 0 && _password[i] == BACKSPACE)
		{
			_password[i] = '\0';
			if (AUTHORITY == ADMIN || _if_have_ADMIN(*_account_head) == FAIL)
				goto REG_ADMIN;
			goto REG_USER;
		}
		else if (_password[i] == BACKSPACE)
		{
		DEL:
			i--;
			printf("\b \b");
		}
		else
		{
			i++;
			printf("*");
			if (i == 19)
			{
				goto DEL;
			}
		}
	}
	if (i == 0 && _password[i] == ENTER)
	{
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
			_move_cursor(2, 6);
			_account_head_copy = (*_account_head);
			printf("密码库中已有相同的密码!");
			DELAY_MAX;
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
		_account_link *_account_node = (_account_link *)malloc(sizeof(_account_link));
		strcpy_s(_account_node->one_account._password, 20, _password);
		if (_authority == ADMIN)
			_account_node->one_account._authority = ADMIN;
		else if (_authority == USER)
			_account_node->one_account._authority = USER;
		_account_node->_next = (*_account_head)->_next;
		(*_account_head)->_next = _account_node;
		_move_cursor(2, 6);
		_save_account(*_account_head);
		printf("密码创建成功!");
		DELAY_MID;
		(*_account_number)++;
	}
}
void _edit_admin_password(_account_link **_account_head, int *_account_number)
{
	int i = 0;
	char _password[20];
	char _select_menu = '*';
EDIT:
	i = 0;
	_account_link *_account_head_copy = (*_account_head);
	if (AUTHORITY == ADMIN)
	{
		_edit_admin_password_interface();
		_move_cursor(5, 6);
		while (_password[i] = getch(), _password[i] != ENTER)
		{
			if (_password[i] == ESC)
			{
				return;
			}
			if (i == 0 && _password[i] == BACKSPACE)
			{
				_password[i] = '\0';
				goto EDIT;
			}
			else if (_password[i] == BACKSPACE)
			{
			DEL:
				i--;
				printf("\b \b");
			}
			else
			{
				i++;
				printf("*");
				if (i == 19)
				{
					goto DEL;
				}
			}
		}
		_password[i] = '\0';
		while (_account_head_copy->_next != NULL)
		{
			if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0 && _account_head_copy->_next->one_account._authority == ADMIN)
			{
				_move_cursor(6, 6);
				printf("按D删除该密码ENTER继续!");
				while (1)
				{
					_select_menu = getch();
					if (_select_menu == 'D' || _select_menu == 'd')
					{
						_account_link *_account_head_copy_free = _account_head_copy->_next;
						_account_head_copy->_next = _account_head_copy->_next->_next;
						free(_account_head_copy_free);
						(*_account_number)--;
						_save_account(*_account_head);
						goto EDIT_INTERFACE;
					}
					else if (_select_menu == ENTER)
					{
					EDIT_B:
						i = 0;
						_move_cursor(5, 7);
						while (_password[i] = getch(), _password[i] != ENTER)
						{
							if (_password[i] == ESC)
							{
								goto EDIT_INTERFACE;
							}
							if (i == 0 && _password[i] == BACKSPACE)
							{
								_password[i] = '\0';
								goto EDIT_B;
							}
							else if (_password[i] == BACKSPACE)
							{
							DEL_B:
								i--;
								printf("\b \b");
							}
							else
							{
								i++;
								printf("*");
								if (i == 19)
								{
									goto DEL_B;
								}
							}
						}
						_password[i] = '\0';
						if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0)
						{
							_move_cursor(6, 6);
							printf("输入的新密码与原密码相同!");
							DELAY_MAX;
							goto EDIT_B;
						}
						else
						{
							strcpy_s(_account_head_copy->_next->one_account._password, 20, _password);
							_move_cursor(6, 6);
							_save_account(*_account_head);
							printf("密码修改成功!");
							DELAY_MIN;
							goto EDIT_INTERFACE;
						}
					}
				}
			}
			else
			{
				_account_head_copy = _account_head_copy->_next;
			}
		}
		_move_cursor(6, 6);
		printf("无该密码!");
		DELAY_MAX;
	}
	else
	{
		_move_cursor(5, 11);
		printf("当前权限不足!");
		DELAY_MAX;
	EDIT_INTERFACE:
		return;
	}
}
void _edit_user_password(_account_link **_account_head, int *_account_number)
{
	int i = 0;
	char _password[20];
	char _select_menu = '*';
EDIT:
	i = 0;
	_account_link *_account_head_copy = (*_account_head);
	_edit_user_password_interface();
	_move_cursor(3, 8);
	while (_password[i] = getch(), _password[i] != ENTER)
	{
		if (_password[i] == ESC)
		{
			return;
		}
		if (i == 0 && _password[i] == BACKSPACE)
		{
			_password[i] = '\0';
			goto EDIT;
		}
		else if (_password[i] == BACKSPACE)
		{
		DEL:
			i--;
			printf("\b \b");
		}
		else
		{
			i++;
			printf("*");
			if (i == 19)
			{
				goto DEL;
			}
		}
	}
	_password[i] = '\0';
	while (_account_head_copy->_next != NULL)
	{
		if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0 && _account_head_copy->_next->one_account._authority == USER)
		{
			_move_cursor(6, 6);
			printf("按D删除该密码ENTER继续!");
			while (1)
			{
				_select_menu = getch();
				if (_select_menu == 'D' && _select_menu == 'd')
				{
					_account_link *_account_head_copy_free = _account_head_copy->_next;
					_account_head_copy->_next = _account_head_copy->_next->_next;
					free(_account_head_copy_free);
					(*_account_number)--;
					_save_account(*_account_head);
					goto EDIT_INTERFACE;
				}
				else if (_select_menu == ENTER)
				{
				EDIT_B:
					i = 0;
					_move_cursor(3, 9);
					while (_password[i] = getch(), _password[i] != ENTER)
					{
						if (_password[i] == ESC)
						{
							goto EDIT_INTERFACE;
						}
						if (i == 0 && _password[i] == BACKSPACE)
						{
							_password[i] = '\0';
							goto EDIT_B;
						}
						else if (_password[i] == BACKSPACE)
						{
						DEL_B:
							i--;
							printf("\b \b");
						}
						else
						{
							i++;
							printf("*");
							if (i == 19)
							{
								goto DEL_B;
							}
						}
					}
					_password[i] = '\0';
					if (strcmp(_account_head_copy->_next->one_account._password, _password) == 0)
					{
						_move_cursor(6, 6);
						printf("输入的新密码与原密码相同");
						DELAY_MAX;
						goto EDIT_B;
					}
					else
					{
						_move_cursor(6, 6);
						strcpy_s(_account_head_copy->_next->one_account._password, 20, _password);
						_save_account(*_account_head);
						printf("密码修改成功!");
						DELAY_MIN;
						goto EDIT_INTERFACE;
					}
				}
			}
		}
		else
		{
			_account_head_copy = _account_head_copy->_next;
		}
	}
	_move_cursor(6, 6);
	printf("无该密码!");
	DELAY_MAX;
EDIT_INTERFACE:;
}
void _user_up(_account_link *_account_head)
{
	if (AUTHORITY == ADMIN)
	{
		_user_up_interface();
		int _error_store = 0;
		char _password[20];
		char _select_judge = '*';
		int i = 0;
	UP:
		i = 0;
		_error_store = 0;
		_move_cursor(9, 10);
		while (_password[i] = getch(), _password[i] != ENTER)
		{
			if (_password[i] == ESC)
			{
				return;
			}
			if (i == 0 && _password[i] == BACKSPACE)
			{
				_password[i] = '\0';
				goto UP;
			}
			else if (_password[i] == BACKSPACE)
			{
			DEL:
				i--;
				printf("\b \b");
			}
			else
			{
				i++;
				printf("*");
				if (i == 19)
				{
					goto DEL;
				}
			}
		}
		if (i == 0 && _password[i] == ENTER)
		{
			goto UP;
		}
		_password[i] = '\0';
		while (_account_head->_next != NULL)
		{
			if (strcmp(_account_head->_next->one_account._password, _password) == 0 && _account_head->_next->one_account._authority == USER)
			{
				_error_store = 1;
				_move_cursor(0, 11);
				printf("ENTER键提升权限");
				_select_judge = getch();
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
						_move_cursor(0, 11);
						printf("  权限提升成功!");
						DELAY_MID;
						goto MAIN_INTERFACE;
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
			_move_cursor(0, 11);
			printf("无此用户");
			DELAY_MAX;
			return;
		}
	}
	else
	{
		_move_cursor(5, 11);
		printf("当前权限不足!");
		DELAY_MAX;
	}
MAIN_INTERFACE:;
}
void _bulk_load(_link **_head_node, int *_number_of_students)
{
	if (AUTHORITY == ADMIN)
	{
		int _option = 0;
		int _error_store = 0;
		int _number = 0; // 用于储存token 读取的数字
		char *token = NULL;
		char _first_char = '*';
		char string[sizeof(_student) + 15 * sizeof(char)];
		_link *_node = NULL;
		strcpy_s(string, sizeof("姓名,学号,性别1男2女,学院,专业,班级,生日 年,月,日,高等数学,程序设计,数据结构,大学英语,大学物理,体育,选修课,\n,,,,,,,,,,,,,,,,#,#放在末尾不能删除"), "姓名,学号,性别1男2女,学院,专业,班级,生日 年,月,日,高等数学,程序设计,数据结构,大学英语,大学物理,体育,选修课,\n,,,,,,,,,,,,,,,,#,#放在末尾不能删除");
		FILE *_fileptr;
		_error_store = fopen_s(&_fileptr, _bluk_import_file_name, "rt");
		if (_error_store != SUCCESS) // 判断文件是否存在，如果不存在则创建，如果存在则不创建
		{
			fopen_s(&_fileptr, _bluk_import_file_name, "wt");
			fwrite(string, sizeof("姓名,学号,性别1男2女,学院,专业,班级,生日 年,月,日,高等数学,程序设计,数据结构,大学英语,大学物理,体育,选修课,\n,,,,,,,,,,,,,,,,#,#放在末尾不能删除"), 1, _fileptr);
			_move_cursor(5, 10);
			fclose(_fileptr);
			printf("请找到文件:student_information_bluk_import.csv(初始位置D盘根目录)");
			_move_cursor(5, 11);
			printf("用Excel打开输入信息");
			getch();
		}
		else
		{
			fseek(_fileptr, 110L, 0);
			_first_char = fgetc(_fileptr);
			rewind(_fileptr);
			fseek(_fileptr, 110L, 0);
			if (_first_char == ',') // 判断是否为空
			{
				_move_cursor(5, 10);
				fclose(_fileptr);
				printf("请找到文件:student_information_bluk_import.csv(初始位置D盘根目录)");
				_move_cursor(5, 11);
				printf("用Excel打开输入信息");
				getch();
			}
			else
			{
				while (fgets(string, sizeof(_student) + 17 * sizeof(char), _fileptr) != NULL)
				{
					_option = 0;
					_node = (_link *)malloc(sizeof(_link));
					token = strtok(string, ",");
					while (token != NULL && strcmp(token, "\n") != 0)
					{
						switch (_option)
						{
						case 0:
							strcpy_s(_node->one_student._name, 20 * sizeof(char), token);
							break;
						case 1:
							strcpy_s(_node->one_student._student_id, 16 * sizeof(char), token);
							break;
						case 2:
							sscanf(token, "%d", &_number);
							_node->one_student._sex = _number;
							break;
						case 3:
							strcpy_s(_node->one_student._school, 40 * sizeof(char), token);
							break;
						case 4:
							strcpy_s(_node->one_student._major, 40 * sizeof(char), token);
							break;
						case 5:
							strcpy_s(_node->one_student._class, 14 * sizeof(char), token);
							break;
						case 6:
							sscanf(token, "%d", &_number);
							_node->one_student._birthday._year = _number;
							break;
						case 7:
							sscanf(token, "%d", &_number);
							_node->one_student._birthday._month = _number;
							break;
						case 8:
							sscanf(token, "%d", &_number);
							_node->one_student._birthday._day = _number;
							break;
						case 9:
							sscanf(token, "%d", &_number);
							_node->one_student._score._advanced_mathematics = _number;
							break;
						case 10:
							sscanf(token, "%d", &_number);
							_node->one_student._score._langue_programming = _number;
							break;
						case 11:
							sscanf(token, "%d", &_number);
							_node->one_student._score._data_struct = _number;
							break;
						case 12:
							sscanf(token, "%d", &_number);
							_node->one_student._score._college_English = _number;
							break;
						case 13:
							sscanf(token, "%d", &_number);
							_node->one_student._score._college_physics = _number;
							break;
						case 14:
							sscanf(token, "%d", &_number);
							_node->one_student._score._physical_education = _number;
							break;
						case 15:
							sscanf(token, "%d", &_number);
							_node->one_student._score._selective_course = _number;
							break;
						}
						token = strtok(NULL, ",");
						_option++;
					}
					_node->one_student._score._GPA = _caculate_gpa(_node->one_student._score._advanced_mathematics, _node->one_student._score._langue_programming, _node->one_student._score._data_struct, _node->one_student._score._college_English, _node->one_student._score._physical_education, _node->one_student._score._physical_education, _node->one_student._score._selective_course);
					_node->_next = (*_head_node)->_next;
					(*_head_node)->_next = _node;
					(*_number_of_students)++;
				}
				_move_cursor(5, 10);
				_save_student_information(*_head_node);
			}
		}
	}
	else
	{
		_move_cursor(5, 9);
		printf("当前权限不足!");
		DELAY_MAX;
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
void _sort_student_information(_link *_head_node)
{
	double a, b;
	_link *_copy = NULL;
	_student _istudent;
	while (_head_node->_next != NULL)
	{
		_copy = _head_node->_next;
		while (_copy->_next != NULL)
		{
			if (sscanf(_head_node->_next->one_student._student_id, "%lf", &a), sscanf(_copy->_next->one_student._student_id, "%lf", &b), a > b)
			{
				_istudent = _head_node->_next->one_student;
				_head_node->_next->one_student = _copy->_next->one_student;
				_copy->_next->one_student = _istudent;
			}
			_copy = _copy->_next;
		}
		_head_node = _head_node->_next;
	}
	_move_cursor(5, 9);
	printf("排序成功!");
	DELAY_MAX;
}
void _move_cursor(int x, int y)
{
	// COORD Coord;
	// Coord.X = XPos;
	// Coord.Y = YPos;
	// SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord);
	printf("\033[%d;%dH", y + 1, x + 1);
}
void _setting()
{
	char _file_copy[200];
	char _file_name[200];
	char _select_menu = '*';
	_setting_interface();
	_select_menu = getch();
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
				_move_cursor(0, 2);
				scanf("%s", _file_name);
				strcpy_s(_file_copy, 200, _student_file_name);
				strcpy_s(_student_file_name, 200, _file_name);
				_setting_save();
				_add_slash(_file_name);
				// MoveFile(_file_copy, _file_name);
			}
			break;
			case 2:
			{
				_move_cursor(0, 4);
				scanf("%s", _file_name);
				strcpy_s(_file_copy, 200, _account_file_name);
				strcpy_s(_account_file_name, 200, _file_name);
				_setting_save();
				_add_slash(_file_name);
				// MoveFile(_file_copy, _file_name);
			}
			break;
			case 3:
			{
				_move_cursor(0, 6);
				scanf("%s", _file_name);
				strcpy_s(_file_copy, 200, _bluk_import_file_name);
				strcpy_s(_bluk_import_file_name, 200, _file_name);
				_setting_save();
				_add_slash(_file_name);
				// MoveFile(_file_copy, _file_name);
			}
			break;
			case 4:
			{
				_move_cursor(8, 10);
				printf("按下回车键确认清空所有文件");
				char ch;
				while (1)
				{
					ch = getch();
					if (ch == ENTER)
					{
						// DeleteFile(_student_file_name);
						// DeleteFile(_account_file_name);
						// DeleteFile(_bluk_import_file_name);
						printf("成功");
						break;
					}
					else if (ch == ESC)
					{
						break;
					}
				}
			}
			break;
			default:;
			}
			_setting_interface();
			_select_menu = getch();
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
	char *token = NULL;
	char *next_token = NULL;
	int _error_store = 0;
	char _first_char = '*';
	FILE *_fileptr = NULL;
	_error_store = fopen_s(&_fileptr, "config.ini", "rb");
	if (_error_store == 0)
	{
		_first_char = fgetc(_fileptr);
		fclose(_fileptr);
	}
	if (_error_store != 0 || _first_char == EOF)
	{
		fopen_s(&_fileptr, "config.ini", "wb");
		strcpy_s(_student_file_name, 200, "student_information.dat");
		strcpy_s(_account_file_name, 200, "account_information.dat");
		strcpy_s(_bluk_import_file_name, 200, "student_information_bluk_import.csv");
		fprintf(_fileptr, "%s", _student_file_name);
		fprintf(_fileptr, "%c", ',');
		fprintf(_fileptr, "%s", _account_file_name);
		fprintf(_fileptr, "%c", ',');
		fprintf(_fileptr, "%s", _bluk_import_file_name);
		fprintf(_fileptr, "%c", ',');
		fclose(_fileptr);
	}
	else
	{
		fopen_s(&_fileptr, "config.ini", "rb");
		fgets(string, sizeof(char) * 700, _fileptr);
		_option = 0;
		token = strtok(string, ",");
		while (_option != 3)
		{
			switch (_option)
			{
			case 0:
				strcpy_s(_student_file_name, 200 * sizeof(char), token);
				break;
			case 1:
				strcpy_s(_account_file_name, 200 * sizeof(char), token);
				break;
			case 2:
				strcpy_s(_bluk_import_file_name, 200 * sizeof(char), token);
				break;
			}
			token = strtok(NULL, ",");
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
			_name[j + 1] = '\0';
			i++;
		}
		i++;
	}
}
void _setting_save()
{
	FILE *_fileptr = NULL;
	fopen_s(&_fileptr, "d:\\config.ini", "wb");
	fprintf(_fileptr, "%s", _student_file_name);
	fprintf(_fileptr, "%c", ',');
	fprintf(_fileptr, "%s", _account_file_name);
	fprintf(_fileptr, "%c", ',');
	fprintf(_fileptr, "%s", _bluk_import_file_name);
	fprintf(_fileptr, "%c", ',');
	fclose(_fileptr);
}
