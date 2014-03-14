#include <stdio.h>
#include <string.h>
#define BUFFER_MAX 100

typedef enum {
	IN_NOTE,
	OUT_NOTE
} Status;

int trans(char *, char *);

int main(int argc, char **argv){
	char buffer[BUFFER_MAX];
	char output[BUFFER_MAX];
	if (argc <= 1) {
		printf("将要转换的文件作为第一个参数传入。\n");
		return 1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	FILE *fp2 = fopen("output.txt","w");
	if (fp == NULL) {
		printf("未能成功打开文件：%s", argv[1]);
		return 1;
	}

	while(fgets(buffer, BUFFER_MAX, fp) != NULL){
		//printf("%s\n", buffer);
		output[0] = '\0';
		int msg = trans(buffer, output);
		//printf("%s",output);
		
		if (msg == 0){
			fputs(output, fp2);
			fputs("\n",fp2);
		}
	}
	fclose(fp);
	fclose(fp2);
	printf("转换完成！为bd2note.exe相同位置的output.txt");
	return 0;
}

int trans(char *src, char *dst){
	int index = 0;
	Status status = OUT_NOTE;
	int sharpFlag = 0;			//是否升调
	
	int hole = 0;				//用于存储孔数
	// char *note[] = {"1","#1","2","#2","3","4","#4","5","#5","6","#6","7"};
	// char *notel[] = {"[1]","[#1]","[2]","[#2]","[3]","[4]","[#4]","[5]","[#5]","[6]","[#6]","[7]"};
	// char *noteh[] = {"(1)","(#1)","(2)","(#2)","(3)","(4)","(#4)","(5)","(#5)","(6)","(#6)","(7)"};
	for(; src[index] != '\0'; ++index){
		//printf("status is %d\n", status);
		if (status == OUT_NOTE){
			if (src[index] == '(')
			{
				if (sharpFlag == 0)
					sharpFlag = 1;
				else
					return 2;	//多余的升音符
				printf("get LB\n");
				continue;
			}
			if (src[index] == ')')
			{
				if (sharpFlag == 1)
					sharpFlag = 0;
				else
					return 2;	//多余的升音符
				printf("get RB\n");
				continue;
			}
			if (src[index] >= '0' && src[index] <= '9'){
				status = IN_NOTE;
				//printf("Status modified: IN_NOTE\n");
				hole = hole * 10 + src[index] - '0';
				printf("get digit\n");
				continue;
			}
			if (isspace(src[index])){
				strncat(dst, " ", 1);
				printf("get space\n");
				continue;
			}
		}
		
		if (status == IN_NOTE){
			if (src[index] == 'B' || src[index] == 'b') {
				printf("get B\n");
				if (sharpFlag){
					switch (hole){
						case 1:strncat(dst, "(#1)", 4);break; 	//1B
						case 2:strncat(dst, "(4)", 3);break;	//2B
						case 3:strncat(dst, "(#5)", 4);break;	//3B
						case 4:strncat(dst, "#1", 2);break;	//4B
						case 5:strncat(dst, "#1", 2);break;	//5B
						case 6:strncat(dst, "4", 1);break;		//6B
						case 7:strncat(dst, "#5", 2);break;	//7B
						case 8:strncat(dst, "[#1]", 4);break;	//8B
						case 9:strncat(dst, "[#1]", 4);break;	//9B
						case 10:strncat(dst, "[4]", 3);break;	//10B
						case 11:strncat(dst, "[#5]", 4);break; //11B
						case 12:strncat(dst, "-#1-", 4);break;	//12B
						default:printf("hole为%d。\n", hole);
					}
				}
				else {
					switch (hole){
						case 1:strncat(dst, "(1)", 3);break; 	//1B
						case 2:strncat(dst, "(3)", 3);break;	//2B
						case 3:strncat(dst, "(5)", 3);break;	//3B
						case 4:strncat(dst, "1", 1);break;		//4B
						case 5:strncat(dst, "1", 1);break;		//5B
						case 6:strncat(dst, "3", 1);break;		//6B
						case 7:strncat(dst, "5", 1);break;		//7B
						case 8:strncat(dst, "[1]", 3);break;	//8B
						case 9:strncat(dst, "[1]", 3);break;	//9B
						case 10:strncat(dst, "[3]", 3);break;	//10B
						case 11:strncat(dst, "[5]", 3);break; //11B
						case 12:strncat(dst, "-1-", 3);break;	//12B
						default:printf("hole为%d。\n", hole);
					} 
				}
				status = OUT_NOTE;
				//printf("Status modified: OUT_NOTE\n");
				hole = 0;
				continue;
			}
			if (src[index] == 'D' || src[index] == 'd') {
				printf("get D\n");
				if (sharpFlag){
					switch (hole){
						case 1:strncat(dst, "(#2)", 4);break; 	//1D
						case 2:strncat(dst, "(#4)", 4);break;	//2D
						case 3:strncat(dst, "(#6)", 4);break;	//3D
						case 4:strncat(dst, "1", 1);break;		//4D
						case 5:strncat(dst, "#2", 2);break;	//5D
						case 6:strncat(dst, "#4", 2);break;	//6D
						case 7:strncat(dst, "#6", 2);break;	//7D
						case 8:strncat(dst, "[1]", 4);break;	//8D
						case 9:strncat(dst, "[#2]", 4);break;	//9D
						case 10:strncat(dst, "[#4]", 4);break;	//10D
						case 11:strncat(dst, "[#6]", 4);break; //11D
						case 12:strncat(dst, "-1-", 3);break;	//12D
						default:printf("hole为%d。\n", hole);
					}
				}
				else {
					switch (hole){
						case 1:strncat(dst, "(2)", 3);break; 	//1D
						case 2:strncat(dst, "(4)", 3);break;	//2D
						case 3:strncat(dst, "(6)", 3);break;	//3D
						case 4:strncat(dst, "(7)", 3);break;		//4D
						case 5:strncat(dst, "2", 1);break;	//5D
						case 6:strncat(dst, "4", 1);break;	//6D
						case 7:strncat(dst, "6", 1);break;	//7D
						case 8:strncat(dst, "7", 1);break;	//8D
						case 9:strncat(dst, "[2]", 3);break;	//9D
						case 10:strncat(dst, "[4]", 3);break;	//10D
						case 11:strncat(dst, "[6]", 3);break; //11D
						case 12:strncat(dst, "[7]", 3);break;	//12D
						default:printf("hole为%d。\n", hole);
					}
				}
				status = OUT_NOTE;
				//printf("Status modified:OUT_NOTE\n");
				hole = 0;
				continue;
			}
			if (src[index] >= '0' && src[index] <= '9'){
				hole = hole * 10 + src[index] - '0';
				printf("get digit\n");
				continue;
			}
		}
	}
	return 0;
}