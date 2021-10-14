#include "Win10_Internet_RepairTool.h"

using namespace std;

static const char* subkeys[] = {
	"ActiveWebProbeContent",
	"ActiveWebProbeContentV6",
	"ActiveWebProbeHost",
	"ActiveWebProbeHostV6",
	"ActiveWebProbePath",
	"ActiveWebProbePathV6",
};

static const char* new_values[] = {
	"Microsoft Connect Test",
	"Microsoft Connect Test",
	"www.msftconnecttest.com",
	"ipv6.msftconnecttest.com",
	"connecttest.txt",
	"connecttest.txt",
};

static const char* old_values[] = {
	"Microsoft NCSI",
	"Microsoft NCSI",
	"www.msftncsi.com",
	"ipv6.msftnsi.com",
	"ncsi.txt",
	"ncsi.txt",
};

int reg_rw(HKEY hKey = NULL, LPCSTR SubKey = NULL, LPCSTR ValueName = NULL, DWORD dwType = REG_SZ, const char* data = "",
	DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_ALL_ACCESS | KEY_WOW64_64KEY) {
	// samDesired 参数需要添加 KEY_WOW64_64KEY ，否则32位程序无法修改64位系统的注册表，会被重定向到 WOW6432Node
	// ValueName、dwType、data 是 RegSetValueEx所需的, 分别是 值的名称、类型、数据
	// const char* data = "" 仅用于 REG_SZ 类型的注册表

	// 创建或打开一个键
	DWORD dwDisposition; // 定义 dwDisposition 用于接收返回码
	auto result = RegCreateKeyEx(hKey, SubKey, 0, NULL, dwOptions, samDesired, NULL, &hKey, &dwDisposition);
	cout << "" << endl;
	if (ERROR_SUCCESS != result) {
		cout << " 创建注册表失败, 错误代码：" << result << endl;
		return -1;
	}
	if (dwDisposition == REG_OPENED_EXISTING_KEY) {
		cout << " 已打开一个存在的注册表项: " << hKey << "\\" << SubKey << "\\" << ValueName << endl;
	}
	else if (dwDisposition == REG_CREATED_NEW_KEY) {
		cout << " 已新建一个注册表项: " << hKey << "\\" << SubKey << "\\" << ValueName << endl;
	}
	// 注意: 后两个参数仅适用于设置 REG_SZ 类型的注册表
	RegSetValueEx(hKey, ValueName, 0, dwType, (unsigned char*)data, strlen(data) * sizeof(data));
	RegCloseKey(hKey);
	return 0;
}


bool is_number(string str)
{
	unsigned int i = 0;
	if (str.c_str()[0] != 45)
	{
		for (i = 0; i < str.length(); i++)
		{
			if (str.c_str()[i] < '0' || str.c_str()[i] > '9')
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (i = 1; i < str.length(); i++)
		{
			if (str.c_str()[i] < '0' || str.c_str()[i] > '9')
			{
				return false;
			}
		}
		return true;
	}
}

int ask_reboot() {
	MessageBox(NULL, "设置已完成, 可能需要在重启后生效。", "提示: ", MB_OK | MB_ICONINFORMATION);
	if (MessageBox(NULL, "是否要立即重启? 记得保存好工作!\n若重启失败, 请自行重启", "重启确认: ", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		system("shutdown -r -t 0");
	}
	return 0;
}

int menu() {
	int i = -1;
	string str;
	system("cls && color cf");
	cout << "\n 注意: \n 仅在使用新版有问题时改为旧版 \n 若有安全软件拦截请放行" << endl << endl;
	cout << " ************* Menu ************* " << endl << endl;
	cout << " 1. 改为旧版" << endl;
	cout << " 2. 改为新版" << endl;
	cout << " 3. 退出程序" << endl;
	cout << "\n ********************************" << endl << endl;
	cout << " 选择(输入数字并回车): ";
	cin >> str;
	if (is_number(str)) {
		i = atoi(str.c_str());
		return i;
	}
	else return -1;
}

int main()
{
	system("title Win10 \"无法连接 Internet\" 问题修复工具 ( v1.0.1 Built by Hui-Shao / 浅蓝的灯) & color cf & mode con cols=130 lines=32");

	int choice = 0;

	while (1) {
		choice = menu();
		switch (choice)
		{
		default:
			cout << " 请重新选择..." << endl;
			system("ping 127.0.0.1 -n 2 >nul");
			break;
		case(1):
			for (unsigned int i = 0; i < sizeof(subkeys) / sizeof(subkeys[0]); i++) {
				reg_rw(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\NlaSvc\\Parameters\\Internet", subkeys[i], REG_SZ, old_values[i]);
			}
			cout << "\n 选项" << choice << "已完成，按任意键继续..." << endl;
			system("color 3f && pause>nul");
			ask_reboot();
			break;
		case(2):
			for (unsigned int i = 0; i < sizeof(subkeys) / sizeof(subkeys[0]); i++) {
				reg_rw(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\NlaSvc\\Parameters\\Internet", subkeys[i], REG_SZ, new_values[i]);
			}
			cout << "\n 选项" << choice << "已完成，按任意键继续..." << endl;
			system("color 3f && pause>nul");
			ask_reboot();
			break;
		case(3):
			exit(0);
		}
	}
	return 0;
}
