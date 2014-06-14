#ifndef STRUCT_H_4DA07D09_5318_4f0f_AE66_4191BBC8A172

#define STRUCT_H_4DA07D09_5318_4f0f_AE66_4191BBC8A172

#pragma pack(push)
#pragma pack(1)

//ѧ����Ϣ��ʽ(gender, birthday, score, �䳤�������� �䳤��ѧ��)
typedef struct tag_student
{
    char           chgender;    //
    unsigned short ushbirth;    //mmdd
    float          fscore;  
    /*
    ��������ı䳤��������ѧ����Ϣ
    */
}tag_student;

//��ӱ��м�¼�ĸ�ʽ(start, totalsize, namesize)
typedef struct tag_addrecord
{
    unsigned long ulstart;      //ѧ����Ϣ���ļ��е�ƫ��
    unsigned char uchsize;      //ѧ����Ϣռ�õ��ܿռ�
    unsigned char uchnamesize;  //ѧ�������ĳ���
    //ѧ��ѧ�ų��� = size - namesize - sizeof(char)
    //               - sizeof(short) - sizeof(float)
}tag_addrecord;

//ɾ�����м�¼�ĸ�ʽ(start, totalsize)
typedef struct tag_delrecord
{
    unsigned long ulstart;
    unsigned char uchsize;
}tag_delrecord;

//�Ƿ��ṩ�ֶ�����Ϣ
typedef struct tag_newinfo
{
    unsigned char gender:1;
    unsigned char birth:1;
    unsigned char score:1;
    unsigned char name:1;
    unsigned char no:1;
    unsigned char infoid:1;
}tag_newinfo;

#pragma pack(pop)

#endif
