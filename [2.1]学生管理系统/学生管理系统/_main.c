#include"_program_all_header.h"
char _student_file_name[200];
char _account_file_name[200];
char _bluk_import_file_name[200];
int AUTHORITY = USER;
void main(void)
{
	system("mode con cols=93 lines=16");//�����͸�
	_startup();
	_setting_initialize();
	Sleep(1000);
	while (_kbhit())
	{
			char ch1 = _getch();
			char ch2 = _getch();
			if (ch1 == 0 && ch2 == 60)
			{
				_setting(); goto LOG_IN;
			}
			else
				break;
	}
	int _number_of_students = 0, _judge = 0;
	char _select = '*';
	int _account_number = 0;
LOG_IN:
	_setting_initialize();
	_number_of_students = 0, _account_number = 0;
	system("mode con cols=93 lines=16");//�����͸�
	_account_link* _account_head = NULL;
	_load_account(&_account_head, &_account_number);
	AUTHORITY = USER;//��ʼ��Ȩ��
	_judge = _start_function(&_account_head, &_account_number);
	if (_judge == FAIL || _judge == EXIT)
	{
		return;
	}
	else
	{
		_number_of_students = 0;                        //ÿ������֮ǰ��ʼ��ѧ������Ϊ0
		_link *_head_node = NULL;
		_load_student_information(&_head_node, &_number_of_students);//�ö���ָ���Ŀ����Ϊ�˸ı��ָ���ֵ
		_main_interface();
		_select = _getch();
		if (_select == ESC)
		{
			if (_head_node->_next != NULL)//�����˳�����½����ʱ �ͷŵ�����������ڴ�
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
			default:RING;
			}
			_main_interface();
			_select = _getch();
			if (_select == ESC)
			{
				goto LOG_IN;//������goto ����������˳�2��ѭ��
				while (_head_node->_next != NULL)//�����˳�����½����ʱ �ͷŵ�����������ڴ�
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