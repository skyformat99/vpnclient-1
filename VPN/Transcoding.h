#pragma once


//ת�����
class CTranscoding
{
public:
	//lua���ַ���ת���ɳ���ʶ���ַ���
	static std::string		LuaString_Procedure(const char* ch);
	static void			LuaString_Procedure(TCHAR* pOutCh, LONG maxlen, const char* ch);	//���SEH����

	//�����ַ���ת����luaʶ���ַ���
	static std::string	Procedure_LuaString(TCHAR* ch);
	static void			Procedure_LuaString(char* pOutCh, LONG maxlen, TCHAR* ch);			//���SEH����

	//ת�������unicode�ַ���ΪLUA�����ʶ���ַ���
	static std::string	TcharToChar(const TCHAR * tchar);

	//LUA�������ַ���ת����tchar�ַ���
	static std::string		CharToTchar(const char * ch);
};
