#include <iostream>
#include<cmath>
using namespace std;
int rom[8192000];
char* regs[32] ;
int pc=0;

void binary_add(int& location, int& sig, int& c, int a, int b, int* arr) {
    //当有进位时：
    if (sig == 1) {
        //确定当前位
        c = a ^ b;
        //当两位相加后，如果不同：0/1
        if (c != sig) {
            //相加：或操作
            c = c | sig;
            //标志位清除
            sig = 0;
        }
        //两位相加后与sig相同：
        else {
            //如果同为1
            if (c == sig == 1) {
                //异或
                c = c ^ sig;
                //有进位
                sig = 1;
            }
            else {
                sig = 0;
            }
        }

    }
    //首次进入或者无进位直接异或得出当前位
    else {
        c = a ^ b;
    }
    //如果都为1进位
    if (a == 1 && b == 1) {
        sig = 1;
    }
    //cout << c << endl;
    //将结果存入另一个数组
    arr[location] = c;
    location += 1;
}
void binary_add(char* b1, char* b2, int len, char* res)
{

    int* bt1, * bt2;
    bt1 = new int[len];
    bt2 = new int[len];
    for (int i = 0;i < len;++i)
    {
        bt1[i] = b1[i]-'0';
        bt2[i] = b2[i]-'0';
        res[i] = '0';
    }
    int c;
    int location = 0;
    int sig = 0;
    int* res0;
    res0 = new int[len + 1];

    for (int i = 0; i < len; i++) 
    {
        binary_add(location, sig, c, (bt1[i]), (bt2[i]), res0);
    }

    for (int i = 0;i < len;++i)
    {
        res[i] = res0[i] + '0';
    }
    delete bt1, bt2, res0;
}

int X_to_D(char* s,int X_len)        
{
    char* tmp;
    tmp = new char[X_len];
    
    int i = 0;
    int result = 0, k = 1;
    for (i = 0;i <X_len;++i)
    { 
        if (s[i] >= '0' && s[i] <= '9') 
        { 
            tmp[i] = s[i] - '0'; 
        } 
        else 
        { 
            switch (s[i])
            { 
            case'a':case 'A':tmp[i] = 10;
            break;
            case'b':case 'B':tmp[i] = 11;
            break;
            case'c':case 'C':tmp[i] = 12;
            break;
            case'd':case 'D':tmp[i] = 13;
            break;
            case'e':case 'E':tmp[i] = 14;
            break;
            case'f':case 'F':tmp[i] = 15;
            break;
            }
        }
        result += long long(tmp[i]) * k;
        k *= 16;
    }
    
    return result;
}
char* D_to_X(int x, char* s)
{
    int i;
    for (i = 0;x != 0;i++)
    {
        s[i] = x % 16;
        if (s[i] <= 9)
        {
            s[i] += '0';
        }
        else
        {
            switch (s[i])
            {
            case 10:
                s[i] = 'A';
                break;
            case 11:
                s[i] = 'B';
                break;
            case 12:
                s[i] = 'C';
                break;
            case 13:
                s[i] = 'D';
                break;
            case 14:
                s[i] = 'E';
                break;
            case 15:
                s[i] = 'F';
                break;
            }
        }
        x /= 16;
    }
    return s;
}
char* X_to_B(char* s, int X_len, char* brr)
{
    int result = X_to_D(s, X_len);
    for (int j = 0;result != 0;j++)
    {
        brr[j] = (result % 2) + '0';
        result /= 2;
    }
    return brr;
}
char* D_to_B(int x, char* bin, int b_len)
{
    for (int i = 0;i < b_len;++i)
        bin[i] = '0';
    if (x < 0)
    {
        bin = D_to_B(-x,bin,b_len);
        for (int i = 0;i < b_len;++i)
        {
            if (bin[i] == '1')
                bin[i] = '0';
            else if (bin[i] == '0')
                bin[i] = '1';
        }
        char tmp[32], one[32];
        for (int i = 0;i < 32;++i)
        {
            one[i] = '0';
            tmp[i] = bin[i];
        }
        one[0] = '1';
        binary_add(tmp, one, 32, bin);
        return bin;
    }
    
    for (int i = 0;x != 0;i++)
    {
        bin[i] = x % 2 +'0';
        x /= 2;
    }
    return bin;
}
int B_to_D(char* s,int s_len,bool sign=false)
{
    if (sign)
    {
        if (s[s_len - 1] == '1')
        {
            char* tmp;
            tmp = new char[s_len];
            
            for (int i = 0;i < s_len;++i)
            {
                if (s[i] == '1')
                    tmp[i] = '0';
                else if (s[i] == '0')
                    tmp[i] = '1';
            }
            int ans= -B_to_D(tmp, s_len, false) - 1;
            delete tmp;
            return ans;
        }
        
    }
        

    int result = 0, deth = 1;
    for (int i = 0; i <s_len; ++i)
    {
        result += (s[i] - '0')* deth;
        deth *= 2;
    }
    return result;
}                   

int view_reg_as_signed(int data)
{
    char bin[32];
    for (int i = 0;i < 32;++i)
        bin[i] = '0';
    D_to_B(data, bin, 32);
    return B_to_D(bin, 32, true);
}
int view_reg_as_unsigned(int data)
{
    char bin[32];
    for (int i = 0;i < 32;++i)
        bin[i] = '0';
    D_to_B(data, bin, 32);
    return B_to_D(bin, 32, false);
}

//各种操作
void LUI(char* bin)//checked
{
    char rd_code[5];
    for (int i = 0;i < 5;++i)
        rd_code[i] = bin[i+7];

    char imm[32];
    for (int i = 0;i < 32;++i)
        imm[i] = '0';
    for (int i = 12;i < 32;++i)
        imm[i] = bin[i];

    int rd = B_to_D(rd_code,5);
    if (rd == 0)
    {
//        cout << hex << ' ' << B_to_D(imm, 32, false) << ' ';
        pc += 4;
        return;
    }
    for (int i = 0;i < 32;++i)
        regs[rd][i] = imm[i];

//    cout << hex <<' '<< B_to_D(imm, 32, false)<< ' ';
    pc += 4;//rom一个元素占一字节 一条命令4字节 所以后移4字节
}

void ADD(char* bin)//checked
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);
    binary_add(regs[rs1], regs[rs2], 32, regs[rd]);

    pc += 4;
}

void SUB(char* bin)//checked
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);

    char tmp[32],one[32],tmp2[32];
    for (int i = 0;i < 32;++i)
    {
        if (i == 0)
            one[i] = '1';
        else
            one[i] = '0';

        if (regs[rs2][i] == '1')
            tmp[i] = '0';
        else
            tmp[i] = '1';
        
    }

    binary_add(regs[rs1], tmp, 32, tmp2);
    binary_add(tmp2, one, 32, regs[rd]);

    pc += 4;
}

void SLT(char* bin)
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);
    if (B_to_D(regs[rs1], 32, true) < B_to_D(regs[rs2], 32, true))
    {
        regs[rd][0] = '1';
        for (int i = 1;i < 32;++i)
            regs[rd][i] = '0';
    }
    else
    {
        for (int i = 0;i < 32;++i)
            regs[rd][i] = '0';
    }

    pc += 4;
}

void SLTU(char* bin)
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);

    if (B_to_D(regs[rs1], 32) < B_to_D(regs[rs2], 32))
    {
        regs[rd][0] = '1';
        for (int i = 1;i < 32;++i)
            regs[rd][i] = '0';
    }
    else
    {
        for (int i = 0;i < 32;++i)
            regs[rd][i] = '0';
    }

    pc += 4;
}

void SLL(char* bin)//checked
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);

    char shift[5];
    for (int i = 0;i < 5;++i)
        shift[i] = regs[rs2][i];
    int shift_amount = B_to_D(shift, 5);
    for (int i = 31;i >=shift_amount;--i)
        regs[rd][i] = regs[rs1][i - shift_amount];
    for (int i = 0;i < shift_amount;++i)
        regs[rd][i] = '0';

    pc += 4;
    
}

void SRL(char* bin)//checked
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);

    char shift[5];
    for (int i = 0;i < 5;++i)
        shift[i] = regs[rs2][i];
    int shift_amount = B_to_D(shift, 5);
    for (int i = 0;i <= 31 - shift_amount;++i)
        regs[rd][i] = regs[rs1][i + shift_amount];
    for (int i = 31;i > 31 - shift_amount;--i)
        regs[rd][i] = '0';
    
    pc += 4;
}

void SRA(char* bin)//checked
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);

    char shift[5];
    for (int i = 0;i < 5;++i)
        shift[i] = regs[rs2][i];
    int top = regs[rs1][31];
    int shift_amount = B_to_D(shift, 5);
    for (int i = 0;i <= 31 - shift_amount;++i)
        regs[rd][i] = regs[rs1][i + shift_amount];
    for (int i = 31;i > 31 - shift_amount;--i)
        regs[rd][i] = top;
    
    pc += 4;
}

void AND(char* bin)
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);
    for (int i = 0;i < 32;++i)
    {
        if (regs[rs1][i] == '1' && regs[rs2][i] == '1')
        {
            regs[rd][i] = '1';
        }
        else
        {
            regs[rd][i] = '0';
        }
    }
    pc += 4;
}

void OR(char* bin)
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);
    char res = '1';
    for (int i = 0;i < 32;++i)
    {
        if (regs[rs1][i] == '1' || regs[rs2][i] == '1')
        {
            regs[rd][i] = '1';
        }
        else
        {
            regs[rd][i] = '0';
        }
    }

    pc += 4;
}

void XOR(char* bin)
{
    char rd_code[5], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    int rd, rs1, rs2;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    rs2 = B_to_D(rs2_code, 5);
    char res = '1';
    for (int i = 0;i < 32;++i)
    {
        if ((regs[rs1][i] == '1' && regs[rs2][i] != '1')||(regs[rs1][i] != '1' && regs[rs2][i] == '1'))
        {
            regs[rd][i] = '1';
        }
        else
        {
            regs[rd][i] = '0';
        }
    }

    pc += 4;
}

void ADDI(char* bin)//checked
{
    char rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    int rd, rs1;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);

    char imm[12],imm_res[32];
    for (int i = 0;i < 12;++i)
    {
        imm[i] = bin[i + 20];
    }
    int imm_num = B_to_D(imm, 12, true);
    D_to_B(imm_num, imm_res, 32);
    binary_add(regs[rs1], imm_res, 32, regs[rd]);

//    cout << hex << ' ' << B_to_D(imm_res,32) << ' ';
    pc += 4;
}

void SLTI(char* bin)
{
    char rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    int rd, rs1;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);

    char imm[12], imm_res[32];
    for (int i = 0;i < 12;++i)
    {
        imm[i] = bin[i + 20];
    }
    int imm_num = B_to_D(imm, 12, true);
    int rs1_num = B_to_D(regs[rs1], 32, true);
    
    char res = '0';
    if (rs1_num < imm_num)
        res = '1';
    regs[rd][0] = res;
    for (int i = 1;i < 32;++i)
    {
        regs[rd][i] = '0';
    }

    pc += 4;
}

void SLTIU(char* bin)
{
    char rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    int rd, rs1;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);

    char imm[12], imm_res[32];
    for (int i = 0;i < 12;++i)
    {
        imm[i] = bin[i + 20];
    }
    int imm_num = B_to_D(imm, 12);
    int rs1_num = B_to_D(regs[rs1], 32);

    char res = '0';
    if (rs1_num < imm_num)
        res = '1';
    regs[rd][0] = res;
    for (int i = 1;i < 32;++i)
    {
        regs[rd][i] = '0';
    }

    pc += 4;
}

void ANDI(char* bin)
{
    char rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    int rd, rs1;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);

    char imm[12], imm_res[32];
    for (int i = 0;i < 12;++i)
    {
        imm[i] = bin[i + 20];
    }
    int imm_num = B_to_D(imm, 12);
    char imm_32[32];
    D_to_B(imm_num, imm_32, 32);

    for (int i = 0;i < 32;++i)
    {
        if (regs[rs1][i] == '1' && imm_32[i] == '1')
        {
            regs[rd][i] = '1';
        }
        else
        {
            regs[rd][i] = '0';
        }
    }
    
    pc += 4;
}

void ORI(char* bin)
{
    char rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    int rd, rs1;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);

    char imm[12], imm_res[32];
    for (int i = 0;i < 12;++i)
    {
        imm[i] = bin[i + 20];
    }
    int imm_num = B_to_D(imm, 12);
    char imm_32[32];
    D_to_B(imm_num, imm_32, 32);

    for (int i = 0;i < 32;++i)
    {
        if (regs[rs1][i] == '1' || imm_32[i] == '1')
        {
            regs[rd][i] = '1';
        }
        else
        {
            regs[rd][i] = '0';
        }
    }

    pc += 4;
}

void XORI(char* bin)
{
    char rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    int rd, rs1;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);

    char imm[12], imm_res[32];
    for (int i = 0;i < 12;++i)
    {
        imm[i] = bin[i + 20];
    }
    int imm_num = B_to_D(imm, 12);
    char imm_32[32];
    D_to_B(imm_num, imm_32, 32);

    for (int i = 0;i < 32;++i)
    {
        if ((regs[rs1][i] == '1' && imm_32[i] != '1') || (regs[rs1][i] != '1' && imm_32[i] == '1'))
        {
            regs[rd][i] = '1';
        }
        else
        {
            regs[rd][i] = '0';
        }
    }

    pc += 4;
}

void SLLI(char* bin)
{
    char rd_code[5], rs1_code[5], shamt_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        shamt_code[i] = bin[i + 20];
    }
    int rd, rs1, shamt;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    shamt = B_to_D(shamt_code, 5);
    for (int i = 31;i >= shamt;--i)
        regs[rd][i] = regs[rs1][i - shamt];
    for (int i = 0;i < shamt;++i)
        regs[rd][i] = '0';

    pc += 4;

}

void SRLI(char* bin)
{
    char rd_code[5], rs1_code[5], shamt_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        shamt_code[i] = bin[i + 20];
    }
    int rd, rs1, shamt;
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    shamt = B_to_D(shamt_code, 5);
    for (int i = 0;i <= 31 - shamt;++i)
        regs[rd][i] = regs[rs1][i + shamt];
    for (int i = 31;i > 31 - shamt;--i)
        regs[rd][i] = '0';

    pc += 4;
}

void SRAI(char* bin)
{
    char rd_code[5], rs1_code[5], shamt_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        shamt_code[i] = bin[i + 20];
    }
    int rd, rs1, shamt;
    
    rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    rs1 = B_to_D(rs1_code, 5);
    int top = regs[rs1][31];
    shamt = B_to_D(shamt_code, 5);
    for (int i = 0;i <= 31 - shamt;++i)
        regs[rd][i] = regs[rs1][i + shamt];
    for (int i = 31;i > 31 - shamt;--i)
        regs[rd][i] = '0';
    regs[rd][31] = top;

    pc += 4;
}

void SB(char* bin)
{
    char offset_code[12], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        offset_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 7;++i)
        offset_code[i+5] = bin[i + 25];

    int rs1 = B_to_D(rs1_code, 5);
    int rs2 = B_to_D(rs2_code, 5);
    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int i = 0;i < 8;++i)
    {
        Bdata_bulk[i] = regs[rs2][i];
    }
    rom[coord] = B_to_D(Bdata_bulk, 8);

    pc += 4;
}

void SH(char* bin)
{
    char offset_code[12], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        offset_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 7;++i)
        offset_code[i + 5] = bin[i + 25];

    int rs1 = B_to_D(rs1_code, 5);
    int rs2 = B_to_D(rs2_code, 5);
    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 2;++j)
    {
        for (int i = 0;i < 8;++i)
        {
            Bdata_bulk[i] = regs[rs2][i + 8 * j];
        }
        rom[coord + j] = B_to_D(Bdata_bulk, 8) ;
    }

    pc += 4;
}

void SW(char* bin)
{
    char offset_code[12], rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        offset_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 7;++i)
        offset_code[i + 5] = bin[i + 25];

    int rs1 = B_to_D(rs1_code,5);
    int rs2 = B_to_D(rs2_code,5);
    int offset = B_to_D(offset_code,12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 4;++j)
    {
        for (int i = 0;i < 8;++i)
        {
            Bdata_bulk[i] = regs[rs2][i+8*j];
        }
        rom[coord +j] = B_to_D(Bdata_bulk, 8) ;
    }
    
//    cout << hex << ' ' << offset << ' ';
    pc += 4;
}

void LW(char* bin)
{
    char offset_code[12], rs1_code[5], rd_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rd_code[i] = bin[i + 7];
    }
    for (int i = 0;i < 12;++i)
    {
        offset_code[i] = bin[i + 20];
    }

    int rs1 = B_to_D(rs1_code, 5);
    int rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }

    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 4;++j)
    {
        D_to_B(rom[coord +j], Bdata_bulk, 8);
        for (int i = 0;i < 8;++i)
        {
            regs[rd][i + 8 * j] = Bdata_bulk[i];
        }
        
    }

    pc += 4;
}

void LH(char* bin)
{
    char offset_code[12], rs1_code[5], rd_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rd_code[i] = bin[i + 7];
    }
    for (int i = 0;i < 12;++i)
    {
        offset_code[i] = bin[i + 20];
    }

    int rs1 = B_to_D(rs1_code, 5);
    int rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 2;++j)
    {
        D_to_B(rom[coord + j], Bdata_bulk, 8);
        for (int i = 0;i < 8;++i)
        {
            regs[rd][i + 8 * j] = Bdata_bulk[i];
        }
    }
    for (int i = 0;i < 16;++i)
    {
        regs[rd][i + 16] = regs[rd][15];
    }

    pc += 4;
}

void LHU(char* bin)
{
    char offset_code[12], rs1_code[5], rd_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rd_code[i] = bin[i + 7];
    }
    for (int i = 0;i < 12;++i)
    {
        offset_code[i] = bin[i + 20];
    }

    int rs1 = B_to_D(rs1_code, 5);
    int rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 2;++j)
    {
        D_to_B(rom[coord + j], Bdata_bulk, 8);
        for (int i = 0;i < 8;++i)
        {
            regs[rd][i + 8 * j] = Bdata_bulk[i];
        }
    }
    for (int i = 0;i < 16;++i)
    {
        regs[rd][i + 16] = 0;
    }

    pc += 4;
}

void LB(char* bin)
{
    char offset_code[12], rs1_code[5], rd_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rd_code[i] = bin[i + 7];
    }
    for (int i = 0;i < 12;++i)
    {
        offset_code[i] = bin[i + 20];
    }

    int rs1 = B_to_D(rs1_code, 5);
    int rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 1;++j)
    {
        D_to_B(rom[coord + j], Bdata_bulk, 8);
        for (int i = 0;i < 8;++i)
        {
            regs[rd][i + 8 * j] = Bdata_bulk[i];
        }
    }
    for (int i = 0;i < 24;++i)
    {
        regs[rd][i + 8] = regs[rd][7];
    }

    pc += 4;
}

void LBU(char* bin)
{
    char offset_code[12], rs1_code[5], rd_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rd_code[i] = bin[i + 7];
    }
    for (int i = 0;i < 12;++i)
    {
        offset_code[i] = bin[i + 20];
    }

    int rs1 = B_to_D(rs1_code, 5);
    int rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += 4;
        return;
    }
    int offset = B_to_D(offset_code, 12, true);

    char offset_32[32], coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    int coord = B_to_D(coord_code, 32, true);

    char Bdata_bulk[8];
    for (int j = 0;j < 1;++j)
    {
        D_to_B(rom[coord + j], Bdata_bulk, 8);
        for (int i = 0;i < 8;++i)
        {
            regs[rd][i + 8 * j] = Bdata_bulk[i];
        }
    }
    for (int i = 0;i < 24;++i)
    {
        regs[rd][i + 8] = 0;
    }

    pc += 4;
}

void JAL(char* bin)
{
    char offset_code[21] = {'0'}, rd_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
    }
    for (int i = 0;i < 10;++i)
    {
        offset_code[i+1] = bin[i + 21];
    }
    for (int i = 0;i < 8;++i)
    {
        offset_code[i+12] = bin[i + 12];
    }
    offset_code[11] = bin[20];
    offset_code[20] = bin[31];

    int offset = B_to_D(offset_code, 21, true);
    int rd = B_to_D(rd_code, 5);
    if (rd == 0)
    {
        pc += offset;
        return;
    }

    D_to_B(pc + 4, regs[rd], 32);

//    cout << hex << ' ' << offset << ' ';

    pc += offset;
}

void JALR(char* bin)
{
    char offset_code[12], rd_code[5], rs1_code[5];
    for (int i = 0;i < 5;++i)
    {
        rd_code[i] = bin[i + 7];
        rs1_code[i] = bin[i + 15];
    }
    for (int i = 0;i < 12;++i)
    {
        offset_code[i] = bin[i + 20];
    }
    int rs1 = B_to_D(rs1_code, 5);
    int rd = B_to_D(rd_code, 5);
    int offset = B_to_D(offset_code, 12, true);
    char offset_32[32],coord_code[32];
    D_to_B(offset, offset_32, 32);
    binary_add(offset_32, regs[rs1], 32, coord_code);
    coord_code[0] = '0';
    int coord = B_to_D(coord_code, 32, true);
    if (rd == 0)
    {
        pc = coord;
        return;
    }
    D_to_B(pc + 4, regs[rd], 32);

    pc = coord;
}

void BEQ(char* bin)
{
    char offset_code[13] = { '0' }, rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 4;++i)
    {
        offset_code[1 + i] = bin[i + 8];
    }
    for (int i = 0;i < 6;++i)
    {
        offset_code[5 + i] = bin[25 + i];
    }
    offset_code[11] = bin[7];
    offset_code[12] = bin[31];

    int rs1 = B_to_D(rs1_code,5);
    int rs2 = B_to_D(rs2_code,5);

    int rs1_num = B_to_D(regs[rs1], 32, true);
    int rs2_num = B_to_D(regs[rs2], 32, true);

    if (rs1_num == rs2_num)
        pc += B_to_D(offset_code, 13, true);
    else
        pc += 4;
}

void BNE(char* bin)
{
    char offset_code[13] = {'0'}, rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 4;++i)
    {
        offset_code[1 + i] = bin[i + 8];
    }
    for (int i = 0;i < 6;++i)
    {
        offset_code[5 + i] = bin[25 + i];
    }
    offset_code[11] = bin[7];
    offset_code[12] = bin[31];

    int rs1 = B_to_D(rs1_code,5);
    int rs2 = B_to_D(rs2_code,5);

    int rs1_num = B_to_D(regs[rs1], 32, true);
    int rs2_num = B_to_D(regs[rs2], 32, true);

    if (rs1_num != rs2_num)
        pc += B_to_D(offset_code, 13, true);
    else
        pc += 4;
}

void BLT(char* bin)
{
    char offset_code[13] = { '0' }, rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 4;++i)
    {
        offset_code[1 + i] = bin[i + 8];
    }
    for (int i = 0;i < 6;++i)
    {
        offset_code[5 + i] = bin[25 + i];
    }
    offset_code[11] = bin[7];
    offset_code[12] = bin[31];

    int rs1 = B_to_D(rs1_code,5);
    int rs2 = B_to_D(rs2_code,5);

    int rs1_num = B_to_D(regs[rs1], 32, true);
    int rs2_num = B_to_D(regs[rs2], 32, true);

    if (rs1_num < rs2_num)
        pc += B_to_D(offset_code, 13, true);
    else
        pc += 4;
}

void BLTU(char* bin)
{
    char offset_code[13] = { '0' }, rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 4;++i)
    {
        offset_code[1 + i] = bin[i + 8];
    }
    for (int i = 0;i < 6;++i)
    {
        offset_code[5 + i] = bin[25 + i];
    }
    offset_code[11] = bin[7];
    offset_code[12] = bin[31];

    int rs1 = B_to_D(rs1_code,5);
    int rs2 = B_to_D(rs2_code,5);

    int rs1_num = B_to_D(regs[rs1], 32);
    int rs2_num = B_to_D(regs[rs2], 32);

    if (rs1_num < rs2_num)
        pc += B_to_D(offset_code, 13, true);
    else
        pc += 4;
}

void BGE(char* bin)
{
    char offset_code[13] = { '0' }, rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 4;++i)
    {
        offset_code[1 + i] = bin[i + 8];
    }
    for (int i = 0;i < 6;++i)
    {
        offset_code[5 + i] = bin[25 + i];
    }
    offset_code[11] = bin[7];
    offset_code[12] = bin[31];

    int rs1 = B_to_D(rs1_code, 5);
    int rs2 = B_to_D(rs2_code, 5);

    int rs1_num = B_to_D(regs[rs1], 32, true);
    int rs2_num = B_to_D(regs[rs2], 32, true);

    if (rs1_num >= rs2_num)
        pc += B_to_D(offset_code, 13, true);
    else
        pc += 4;
}

void BGEU(char* bin)
{
    char offset_code[13] = { '0' }, rs1_code[5], rs2_code[5];
    for (int i = 0;i < 5;++i)
    {
        rs1_code[i] = bin[i + 15];
        rs2_code[i] = bin[i + 20];
    }
    for (int i = 0;i < 4;++i)
    {
        offset_code[1 + i] = bin[i + 8];
    }
    for (int i = 0;i < 6;++i)
    {
        offset_code[5 + i] = bin[25 + i];
    }
    offset_code[11] = bin[7];
    offset_code[12] = bin[31];

    int rs1 = B_to_D(rs1_code, 5);
    int rs2 = B_to_D(rs2_code, 5);

    int rs1_num = B_to_D(regs[rs1], 32);
    int rs2_num = B_to_D(regs[rs2], 32);

    if (rs1_num >= rs2_num)
        pc += B_to_D(offset_code, 13, true);
    else
        pc += 4;
}



//解码指令 得到opcode 执行相应操作
void process_inst(char* bin)
{
    cout << hex << pc << ' ';


    char opcode[7],func[3],func2[7];
    int inst_num,func_num,func2_num;
    for (int i = 0;i < 7;++i)
    {
        opcode[i] = bin[i];
    }
    for (int i = 0;i < 3;++i)
    {
        func[i] = bin[i + 12];
    }
    for (int i = 0;i < 7;++i)
    {
        func2[i] = bin[i + 25];
    }
    inst_num = B_to_D(opcode,7);
    func_num = B_to_D(func, 3);
    func2_num = B_to_D(func2, 7);
    switch (inst_num) 
    {
        
        case 0b0110111:
            cout << "LUI ";
            LUI(bin);
            break;
        case 0b1101111:
            cout << "JAL ";
            JAL(bin);
            break;
        case 0b1100111:
            cout << "JALR ";
            JALR(bin);
            break;
        case 0b1100011:
        {
            switch (func_num)
            {
            case 0b000:
                cout << "BEQ ";
                BEQ(bin);
                break;
            case 0b001:
                cout << "BNE ";
                BNE(bin);
                break;
            case 0b100:
                cout << "BLT ";
                BLT(bin);
                break;
            case 0b101:
                cout << "BGE ";
                BGE(bin);
                break;
            case 0b110:
                cout << "BLTU ";
                BLTU(bin);
                break;
            case 0b111:
                cout << "BGEU ";
                BGEU(bin);
                break;
            }
            break;
        }
        case 0b0000011:
        {
            switch (func_num)
            {
            case 0b000:
                cout << "LB ";
                LB(bin);
                break;
            case 0b001:
                cout << "LH ";
                LH(bin);
                break;
            case 0b010:
                cout << "LW ";
                LW(bin);
                break;
            case 0b100:
                cout << "LBU ";
                LBU(bin);
                break;
            case 0b101:
                cout << "LHU ";
                LHU(bin);
                break;
            }
            break;
        
        }
        case 0b0100011:
        {
            switch (func_num)
            {
            case 0b000:
                cout << "SB ";
                SB(bin);
                break;
            case 0b001:
                cout << "SH ";
                SH(bin);
                break;
            case 0b010:
                cout << "SW ";
                SW(bin);
                break;
            }
            break;

        }
        case 0b0010011:
        {
            switch (func_num)
            {
            case 0b000:
                cout << "ADDI ";
                ADDI(bin);
                break;
            case 0b010:
                cout << "SLTI ";
                SLTI(bin);
                break;
            case 0b011:
                cout << "SLTIU ";
                SLTIU(bin);
                break;
            case 0b100:
                cout << "XORI ";
                XORI(bin);
                break;
            case 0b110:
                cout << "ORI ";
                ORI(bin);
                break;
            case 0b111:
                cout << "ANDI ";
                ANDI(bin);
                break;
            case 0b001:
                cout << "SLLT ";
                SLLI(bin);
                break;
            case 0b101:
            {
                if (func2_num == 0b0000000)
                {
                    cout << "SRLI ";
                    SRLI(bin);
                }
                    
                else if (func2_num == 0b0100000)
                {
                    cout << "SRAI ";
                    SRAI(bin);
                }
                    
                break;
            }

            }
            break;
            
        }
        case 0b0110011:
        {
            switch (func_num)
            {
            case 0b000:
            {
                if (func2_num == 0b0000000)
                {
                    cout << "ADD ";
                    ADD(bin);
                }
                    
                else if (func2_num == 0b0100000)
                {
                    cout << "SUB ";
                    SUB(bin);
                }
                    
                break;
            }
            case 0b001:
                cout << "SLL ";
                SLL(bin);
                break;
            case 0b010:
                cout << "SlT ";
                SLT(bin);
                break;
            case 0b011:
                cout << "SLTU ";
                SLTU(bin);
                break;
            case 0b100:
                cout << "XOR ";
                XOR(bin);
                break;
            case 0b101:
            {
                if (func2_num == 0b0000000)
                {
                    cout << "SRL ";
                    SRL(bin);
                }
                    
                else if (func2_num == 0b0100000)
                {
                    cout << "SRA ";
                    SRA(bin);
                }
                    
                break;
            }
            case 0b110:
                cout << "OR ";
                OR(bin);
                break;
            case 0b111:
                cout << "AND ";
                AND(bin);
                break;
            }
            break;

        }
       
       

    }

    
    cout <<hex<<'\t'<< B_to_D(bin, 32)<<endl;

}
//stdin读入模拟内存
bool read_in()
{
    
    static char ch = cin.get();
    if (ch == EOF)
        return false;
    while (ch!= EOF)
    {
        char add[8],rub[12];
        if (ch == '@')
        {
            for (int i = 0;i < 8;++i)
            {
                ch = cin.get();
                add[7 - i] = ch;
            }
            pc = X_to_D(add,8);
        }
            
        else if (ch == '\n')
        {
            ch=cin.get();
            continue;
        }
        else if (ch == ' ')
        {
            ch=cin.get();
            continue;
        }
        else
        {
            char ch0 = cin.get();
            char chs[2];
            chs[0] = ch0;
            chs[1] = ch;
            rom[pc] = X_to_D(chs,2);
            pc += 1;
        }
        ch = cin.get();
    }
    pc = 0;
    if (ch == EOF)
        return false;
    return true;
}

//从模拟内存得到一条指令,转为二进制串，最低位在右
bool get_inst(char* operation)
{
    char bin1[8], bin2[8], bin3[8], bin4[8];
    for (int i = 0;i < 8;++i)
    {
        bin1[i] = '0';
        bin2[i] = '0';
        bin3[i] = '0';
        bin4[i] = '0';
    }
    int d1, d2, d3, d4;
    d1 = rom[pc];
    d2 = rom[pc + 1];
    d3 = rom[pc + 2];
    d4 = rom[pc + 3];
    D_to_B(rom[pc], bin1 ,8);
    D_to_B(rom[pc + 1], bin2 ,8);
    D_to_B(rom[pc + 2], bin3 ,8);
    D_to_B(rom[pc + 3], bin4, 8);

    if (d4 == 15 && d3 == 240 && d2 == 5 && d1 == 19)
    {
        cout << endl << dec << B_to_D(regs[10], 32);
        return false;
    }
        
        
    for (int i = 0;i < 8;++i)
        operation[i] = bin1[i];
    for (int i = 0;i < 8;++i)
        operation[i+8] = bin2[i];
    for (int i = 0;i < 8;++i)
        operation[i+16] = bin3[i];
    for (int i = 0;i < 8;++i)
        operation[i+24] = bin4[i];

    

    
    return true;
    
}


int main()
{
    /*
    char operation[32] = { '1','1','1','1','1','1','1','0','1','0','0','1','0','1','0','0','0','0','0','1','1','0','1','0','1','1','1','0','0','0','1','1' };
    BNE(operation);
    */

    char operation[32];
    for (int i = 0;i < 8192000;++i)
        rom[i] = 0;
    
    for (int i = 0;i < 32;++i)
    {
        regs[i] = new char[32];
        for (int j = 0;j < 32;++j)
            regs[i][j] = '0';
    }
        
    read_in();


    while(get_inst(operation))
    {
        process_inst(operation);
//        cout << dec<<"    "<< B_to_D(regs[10],32)<<"     "<<hex;
    }

    

}


