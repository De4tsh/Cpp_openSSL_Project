#include <iostream>
#include <openssl/md5.h>
#include <fstream>
#include <thread>
#define __BLOCK_SIZE__	1024	// һ�ζ�ȡ�����ֽ�
#define __BUF_SIZE__	2048
#define __MD5_OUT__		256
using namespace std;
string fnGetFileListHash(string filepath)
{
	string hash;
	// �Զ����Ƶķ�ʽ�� ���ı���ʽ�򿪿���ֻ���˸�ʽ�ı�û�仯
	ifstream ifs(filepath, ios::binary);
	if (!ifs)
	{
		return hash;
	}

	// �ļ���ȡ����Buf
	unsigned char buf[__BUF_SIZE__] = {0};

	// Hash���
	unsigned char out[__MD5_OUT__] = { 0 };

	while (!ifs.eof()) // ��û��ȡ���ļ�ĩβ
	{
		ifs.read((char*)buf, __BLOCK_SIZE__);

		int read_size = ifs.gcount(); // �Ѷ�ȡ������

		if (read_size <= 0)
		{
			break;
		}

		MD5(buf, read_size, out);

		hash.insert(hash.end(), out, out + 16);
		// MD5��ĳ���Ϊ16�ֽڣ����Խ�βָ��Ϊ out + 16
	}

	ifs.close();

	// �������п��ϣ���ܹ�ϣ
	MD5((unsigned char*)hash.data(), hash.size(), out); 

	return string(out, out + 16); // ��������Hash
	
}

void PrintfHEX(string data)
{
	cout << "The Hash of the FILE you indicate is :" << endl;

	for (auto c : data)
	{
		cout << hex << (int)(unsigned char)c; // ���з���λintǿת��Ჹλ
	}

	cout << endl;
}


int main(int argc, char** argv)
{
	unsigned char cData[] = "����MD5����";
	unsigned char cOut[1024];
	int len = sizeof(cData);

	MD5_CTX c;
	/*
	typedef struct MD5state_st
	{
		MD5_LONG A, B, C, D;
		MD5_LONG Nl, Nh;
		MD5_LONG data[MD5_LBLOCK];
		unsigned int num;

	} MD5_CTX;
	*/

	MD5_Init(&c);

	MD5_Update(&c, cData, len);

	MD5_Final(cOut, &c);

	cout << "MD5�������Ϊ��" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << hex << (int)cOut[i];
	}

	cout << endl;

	string filepath = "../../src/First_OpenSSL/MD5Hash.cpp";

	auto hash_old = fnGetFileListHash(filepath);
	PrintfHEX(hash_old);


	// ��֤�ļ�������
	for (;;)
	{
		auto hash_new = fnGetFileListHash(filepath);
		
		if (hash_old != hash_new)
		{
			
			cout << "�ļ����޸�" << endl;
			hash_old = hash_new;
			PrintfHEX(hash_new);
		}

		this_thread::sleep_for(1s);
	}
}

	