#ifndef REPSTR_H
#define REPSTR_H

/**
 * @brief ���Դ�С�������ִ��ь���ʽ
 *
 * �˺�ʽ�ڽo�����ִ� haystack ���ь����Դ�С�������ִ� needle��
 *
 * @param haystack ԭʼ�ִ����ڴ��ִ����ь����ִ���
 * @param needle Ҫ�ь������ִ���
 * @return char* ����ָ�� haystack �е�һ�γ��F�����ִ� needle ��ָ�ˣ����δ�ҵ��t���� NULL��
 */
char* strcasestr(const char* haystack, const char* needle);

/**
 * @brief �ִ�ȡ����ʽ
 *
 * �@����ʽ�Á��ڽo�����ִ��Ќ����г��F�����ִ���Q����һ���ִ���
 *
 * @param str ԭʼ�ִ�
 * @param sub ��Ҫ����Q�����ִ�
 * @param rep �Á���Q���ִ�
 * @param ignoreCase �Ƿ���Դ�С��������ֵ��ʾ���Դ�С�������ʾ�^�ִ�С����
 * @return �ɹ��r���� 0
 */
int strReplace(char* str, char* sub, char* rep, int ignoreCase);

#endif // REPSTR_H
