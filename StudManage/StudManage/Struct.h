#ifndef STRUCT_H_4DA07D09_5318_4f0f_AE66_4191BBC8A172

#define STRUCT_H_4DA07D09_5318_4f0f_AE66_4191BBC8A172

#pragma pack(push)
#pragma pack(1)

//学生信息格式(gender, birthday, score, 变长的姓名， 变长的学号)
typedef struct tag_student
{
    char           chgender;    //
    unsigned short ushbirth;    //mmdd
    float          fscore;  
    /*
    后面紧跟的变长的姓名、学号信息
    */
}tag_student;

//添加表中记录的格式(start, totalsize, namesize)
typedef struct tag_addrecord
{
    unsigned long ulstart;      //学生信息在文件中的偏移
    unsigned char uchsize;      //学生信息占用的总空间
    unsigned char uchnamesize;  //学生姓名的长度
    //学生学号长度 = size - namesize - sizeof(char)
    //               - sizeof(short) - sizeof(float)
}tag_addrecord;

//删除表中记录的格式(start, totalsize)
typedef struct tag_delrecord
{
    unsigned long ulstart;
    unsigned char uchsize;
}tag_delrecord;

//是否提供字段新信息
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
