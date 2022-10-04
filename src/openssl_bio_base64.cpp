#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <time.h>
using namespace std;
#define __BUF_SIZE__	1024


// ����
int Base64Encode(const unsigned char* in, int len, char* out_base64)
{

	if (!in || len <= 0 || !out_base64)
	{
		return 0;
	}

	// �ڴ�Ԫ source
	auto mem_bio = BIO_new(BIO_s_mem());
	if (!mem_bio)
	{
		return 0;
	}

	// Base64������
	auto b64_bio = BIO_new(BIO_f_base64());
	if (!b64_bio)
	{
		BIO_free(mem_bio);
		return 0;
	}

	// �γ�BIO��
	// b64 - mem
	BIO_push(b64_bio, mem_bio);

	// ����64�ֽڲ���ӻ��з���\n���������������һ����
	BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);

	// д�뵽b64 fileter���б��룬����ᴫ�ݵ��������һ���ڵ�
	// ��mem��ȡ�����������ͷ����ȡ ���� ����ͷ��������������
	// BIO_write��ʾ����
	// ��������ÿ64�ֽڣ���ȷ������� \n ���з������ܶԽ������Ӱ��
	int re = BIO_write(b64_bio, in, len);
	if (re <= 0)
	{
		BIO_free_all(b64_bio);
		// �����������ڵ�
		return 0;
	}

	// ˢ�»�����,д�������mem
	BIO_flush(b64_bio);


	int outsize = 0;
	// ������Դ�ڴ��ȡ
	BUF_MEM* p_data = 0;
	BIO_get_mem_ptr(b64_bio, &p_data);
	if (p_data)
	{
		memcpy(out_base64, p_data->data, p_data->length);
		outsize = p_data->length;
	}

	BIO_free_all(b64_bio);
	return outsize;
}

// ���� 4�ֽ�ת3�ֽ�
int Base64Decode(const char* in, int len, unsigned char* out_data)
{
	if (!in || len < 0 || !out_data)
	{
		return 0;
	}

	// �ڴ�Դ�������ģ�
	// ����
	auto mem_bio = BIO_new_mem_buf(in, len);
	if (!mem_bio)
	{
		return 0;
	}

	// base64������
	auto b64_bio = BIO_new(BIO_f_base64());
	if (!b64_bio)
	{
		BIO_free(mem_bio);
		return 0;
	}

	// �γ�BIO��
	BIO_push(b64_bio, mem_bio);


	// Ĭ�϶�ȡ���з������������������з�Ĭ�ϻ��������Ҳ�����������
	BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);
	// ��ʱ����������� \n ��ʧ��

	// ��ȡ����
	// ͨ����д�ж��ǽ��뻹���� ����λ����
	// �����Ĵ�С�޷����㣬��Ҫ������������
	size_t size = 0;
	BIO_read_ex(b64_bio, out_data,len ,&size); // ͨ��size���ؽ����Ĵ�С
	BIO_free_all(b64_bio);
	return size;
}



//int main()
//{
//
//
//	unsigned char data[] = "true ���մ��� [left,mid] ������ right = mid";
//	char outdata[__BUF_SIZE__] = {0};
//	unsigned char outdata2[__BUF_SIZE__] = { 0 };
//	int len = sizeof(data);
//	size_t re;
//
//	int nLength = Base64Encode(data, len, outdata);
//
//	if (nLength > 0)
//	{
//		outdata[nLength] = '\0';
//	}
//
//	cout << "����������Ϊ��";
//	cout << outdata << endl;
//
//	re = Base64Decode(outdata, nLength, outdata2);
//
//	cout << "�����ˣ�" << re << "���ֽڣ�ԭ��Ϊ��" << outdata2 << endl;
//
//	return 0;
//
//
//}