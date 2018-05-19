//  main.cpp
//  ThreeCancellation
//
//  Created by LC on 2016/12/4.
//  Copyright © 2016年 LC. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <time.h>
using namespace std;

//Using ♦ for the red un-cancellable blocks
//Using ? for the yellow un-swappable blocks

int ROUND = 1;
int starter = 0;
int Countround = 0;
int WarorDue = 1;
int isvalid = -1;
int iswin = 0;
int isrestart = 0;
int ishealth = 0;
int isarmor = 0;
int ismana = 0;
int Duearmor;
int WarriorHealth;
int WarriorArmor;
int WarriorMana;

class WARRIOR
{
public:
    int WarHealth = 8;
    int WarArmor;
    int WarMana = 50;
    WARRIOR(int Warhealth,int x)
    {
        WarHealth = Warhealth;
    }
    WARRIOR(int Wararmor,int x, int y)
    {
        WarArmor = Wararmor;
    }
    WARRIOR(int Warmana, char c)
    {
        WarMana = Warmana;
    }
};

class DUE
{
public:
    int DueArmor = 60;
    int DueMove;
    DUE(int Duearmor,int y)
    {
        DueArmor = Duearmor;
    }
};

class BLOCK
{
public:
    int blockvalue;
    int blocktype;
    int blockpos;
    int ismoved;
    int isjustmoved;
    int istocan;
};

class BOARD
{
public:
    int Row = 8;
    int Colomn = 8;
    BOARD(int row, int colomn)
    {
        Row = row;
        Colomn = colomn;
    }
};

int Blocktype(int blockvalue)
{
    int type;
    if (blockvalue <=0)  //"-"
        type = -1;
    else if (blockvalue <= 31)  //"1"
        type = 1;
    else if (blockvalue <= 63)  //"0"
        type = 2;
    else if (blockvalue <= 95)  //"*"
        type = 3;
    else if (blockvalue <= 97)  //"#"
        type = 1;
    else if (blockvalue <= 99)  //"@"
        type = 3;
    else if (blockvalue == 100) //"♦"
        type = 6;
    else if (blockvalue > 100)  //"?"
        type = 7;
    return type;
}

string Print(int blockvalue)
{
    string block;
    if (blockvalue <=0)
        block = "-";
    else if (blockvalue <= 31)
        block = "1";
    else if (blockvalue <= 63)
        block = "0";
    else if (blockvalue <= 95)
        block = "*";
    else if (blockvalue <= 97)
        block = "#";
    else if (blockvalue <= 99)
        block = "@";
    else if (blockvalue == 100)
        block = "♦";
    else if (blockvalue > 100)
        block = "?";
    return block;
}

void Draw(int ROW, int COLOMN, vector<BLOCK>&board,WARRIOR &warrior,DUE &due)
{
    if (starter == 1)
        Countround++;
    if (Countround > 8)
        for (int i = 0; i < ROW*COLOMN; i++)
            if (board[i].blocktype > 100)
                board[i].blocktype -= 100;
    cout << "DUE        " << "Armor: " << due.DueArmor << endl;
    cout << "Warrior    " << "Health: " << warrior.WarHealth
    << "    Armor: " << warrior.WarArmor << "   Mana: "
    << warrior.WarMana << endl;
    cout << "Round " << ROUND;
    if (WarorDue == 1)
        cout << "    Warrior" << endl;
    //first line
    cout << "  " << "|";
    cout.setf(ios::left);
    for (int i = 0; i < COLOMN; i++)
        cout << setw(4) << i+1;
    cout << endl;
    //second line
    cout << "--" << "+";
    for (int i = 0; i < COLOMN; i++)
        cout << "----";
    cout << endl;
    //the next lines
    int n = 0,i,j;
    for (i = 0; i < ROW - 1; i++)
    {
        if (i <= 8)
            cout << " ";
        cout << i+1 << "|";
        for (j = 0; j < COLOMN; j++)
            cout << Print(board[n++].blockvalue) << "   ";
        cout << endl << "  |" << endl;
    }
    if (ROW <= 8)
        cout << " ";
    cout << i+1 << "|";
    for (j = 0; j < COLOMN; j++)
        cout << Print(board[n++].blockvalue) << "   ";
    cout << endl;
}

void Duemove(int ROW, int COLOMN,
             WARRIOR &warrior, DUE &due, vector<BLOCK>&board)
{
    //Generate random due moves
    due.DueMove = rand() % 100 + 1;
    if (due.DueMove <= 45)
    {
        int attack = rand() % 4 + 3 + ROUND / 8;
        warrior.WarHealth -= attack;
        cout << "Due deals " << attack
        << " attacks on the Warrior." << endl;
    }
    else if (due.DueMove <= 90)
    {
        int armor = rand() % 4 + 3;
        due.DueArmor += armor;
        cout << "Due adds " << armor << " armors." << endl;
    }
    else if (due.DueMove <= 95)
    {
        //if to change 1 or 2 blocks
        int temp = rand() % 4 + 1;
        if (temp <= 3)
        {
            int pos = 0;
            if(board[pos].blocktype==100) pos++;
            while (board[pos].blocktype == 100)
                pos = rand() % (ROW*COLOMN) + 1;
            board[pos].blocktype = 100;
        }
        if (temp == 4)
        {
            int pos1 = 0, pos2 = 0;
            if (board[pos1].blocktype==100) pos1++;
            pos2 = pos1;
            while (board[pos1].blocktype==100 || board[pos2].blocktype==100 || pos1 == pos2)
            {
                pos1 = rand() % (ROW*COLOMN) + 1;
                pos2 = rand() % (ROW*COLOMN) + 1;
            }
            board[pos1].blocktype = 100;
            board[pos2].blocktype = 100;
        }
    }
    else if (due.DueMove <= 100)
    {
        //set the starter to start counting rounds
        starter = 1;
        Countround = 0;
        //Generate 8 different random positions
        //Generate the coordinates
        vector<int> coor;
        for (int i = 0; i < ROW*COLOMN; i++)
            coor.push_back(i+1);
        int pos;
        for (int i = 0; i < 8; i++)
        {
            pos = rand() % (ROW*COLOMN) + 1;
            if (coor[pos] != 0)
            {
                board[pos].blocktype += 100;
                coor[pos] = 0;
            }
        }
    }
}

void SquareBomb(BLOCK&block,vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &warrior,DUE &due);
void CrossBomb(BLOCK block,vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &Warrior, DUE &Due);

void Effects(vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &Warrior,DUE &Due,BLOCK block)
{
    if (block.blockvalue>=1 && block.blockvalue<=31)
        Due.DueArmor--;
    else if (block.blockvalue>=32 && block.blockvalue<=63)
        Warrior.WarArmor++;
    else if (block.blockvalue>=64 && block.blockvalue<=95)
        Warrior.WarMana++;
    else if (block.blockvalue>=96 && block.blockvalue<=97)
        CrossBomb(block, board, ROW, COLOMN, Warrior, Due);
    else if (block.blockvalue>=98 && block.blockvalue<=99)
        SquareBomb(block, board, ROW, COLOMN, Warrior, Due);
}

void SquareBomb(BLOCK&block,vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &warrior,DUE &due)
{
    board[block.blockpos-1].blocktype = -1;
    board[block.blockpos-1].blockvalue = -1;
    int row = (block.blockpos-1) / COLOMN + 1;
    int colomn = (block.blockpos-1) % COLOMN + 1;
    if (row>1 && row<ROW && colomn>1 && colomn<COLOMN)  //the bomb is not at margins
    {
        for (int i=0; i<3; i++)
            for (int ii=0; ii<3; ii++)
            {
                Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
            }
    }
    else
    {
        if (row == 1 && colomn == 1)
            for (int i=1; i<3; i++)
                for (int ii=1; ii<3; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row == 1 && colomn == COLOMN)
            for (int i=1; i<3; i++)
                for (int ii=0; ii<2; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row == ROW && colomn == 1)
            for (int i=0; i<2; i++)
                for (int ii=1; ii<3; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row == ROW && colomn == COLOMN)
            for (int i=0; i<2; i++)
                for (int ii=0; ii<2; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row == 1 && colomn > 1 && colomn < COLOMN)
            for (int i=1; i<3; i++)
                for (int ii=0; ii<3; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row == ROW && colomn > 1 && colomn < COLOMN)
            for (int i=0; i<2; i++)
                for (int ii=0; ii<3; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row > 1 && row < ROW && colomn == 1)
            for (int i=0; i<3; i++)
                for (int ii=1; ii<3; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
        if (row > 1 && row < ROW && colomn == COLOMN)
            for (int i=0; i<3; i++)
                for (int ii=0; ii<2; ii++)
                {
                    Effects(board, ROW, COLOMN, warrior, due, board[block.blockpos-2-COLOMN+COLOMN*i+ii]);
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].istocan = 1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blocktype = -1;
//                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].blockvalue = -1;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].isjustmoved++;
                    board[block.blockpos-2-COLOMN+COLOMN*i+ii].ismoved++;
                }
    }
}

void CrossBomb(BLOCK block,vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &Warrior, DUE &Due)
{
    board[block.blockpos-1].blockvalue = -1;
    board[block.blockpos-1].blocktype = -1;
    int row = (block.blockpos-1) / COLOMN + 1;
    int colomn = (block.blockpos-1) % COLOMN + 1;
    for (int i = (row-1)*COLOMN; i < row*COLOMN; i++)
    {
        if (i != block.blockpos-1)
        {
            Effects(board, ROW, COLOMN, Warrior, Due, board[i]);
            board[i].istocan = 1;
    //        board[i].blocktype = -1;
    //        board[i].blockvalue = -1;
            board[i].ismoved++;
            board[i].isjustmoved = 1;
        }
        else continue;
    }
    for (int i = colomn-1; i < (ROW-1)*COLOMN+colomn-1; i+=COLOMN)
    {
        if (i != block.blockpos-1)
        {
            Effects(board, ROW, COLOMN, Warrior, Due, board[i]);
            board[i].istocan = 1;
    //        board[i].blocktype = -1;
    //        board[i].blockvalue = -1;
            board[i].ismoved++;
            board[i].isjustmoved = 1;
        }
    }
}

void Fall(vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &Warrior,DUE &Due)
{
    for (int i = 0; i < ROW*COLOMN; i++)
        //if the empty block is not in the first line, let the upper blocks fall
        if (board[i].blocktype == -1)
        {
            if (i < COLOMN)
            {
                board[i].blockvalue = rand() % 100 + 1;
                board[i].blocktype = Blocktype(board[i].blockvalue);
            }
            if (i >= COLOMN)
            {
                int ii;
                for (ii=i-COLOMN;ii>=0;ii-=COLOMN)
                {
                    board[ii].ismoved++;
                    board[ii].isjustmoved = 1;
                    board[ii+COLOMN] = board[ii];
                }
                board[ii+COLOMN].blockvalue = -1;
                board[ii+COLOMN].blocktype = -1;
                board[ii+COLOMN].isjustmoved = 1;
                board[ii+COLOMN].ismoved++;
                for (ii = i-COLOMN;ii>=COLOMN;ii-=COLOMN)
                {
                    board[ii].blockpos += i+1;
                }
            }
        }
    for (int i = 0; i < ROW*COLOMN; i++)
        if (board[i].blockvalue == -1)
        {
            board[i].blockvalue = rand() % 100 + 1;
            board[i].blocktype = Blocktype(board[i].blockvalue);
        }
//    Draw(ROW, COLOMN, board, Warrior, Due);
//    sleep(2);
//    for (int i = 0; i < ROW*COLOMN; i++)
//        if (board[i].blocktype==-1)
//        {
//            board[i].blockvalue = rand() % 100 + 1;
//            board[i].blocktype = Blocktype(board[i].blockvalue);
//        }
}

void FindCancellation(BLOCK block,vector<BLOCK>&board,int ROW,
                      int COLOMN,WARRIOR &Warrior,DUE &Due)
{
    int tocan;
    int row = (block.blockpos-1)/COLOMN+1;
    int col = (block.blockpos-1)%COLOMN+1;
    //look for the right
    int rightpos = 0;
    for (int i=block.blockpos;i<COLOMN*row;i++)
    {
        if (board[i].blocktype == block.blocktype)
        {
            rightpos++;
            isvalid++;
        }
        else
            break;
    }
    //look for the left
    int leftpos = 0;
    for (int i=block.blockpos-2;i>=COLOMN*(row-1);i--)
    {
        if (board[i].blocktype == block.blocktype)
        {
            leftpos++;
            isvalid++;
        }
        else
            break;
    }
    if (rightpos + leftpos >= 2)
    {
        for (int i=0;i<=leftpos;i++)
        {
            Effects(board, ROW, COLOMN, Warrior, Due, block);
            board[block.blockpos-1-i].istocan = 1;
//            board[block.blockpos-1-i].blocktype = -1;
//            board[block.blockpos-1-i].blockvalue = -1;
        }
        for (int i=0;i<=rightpos;i++)
        {
            Effects(board, ROW, COLOMN, Warrior, Due, block);
            board[block.blockpos-1+i].istocan = 1;
//            board[block.blockpos-1+i].blocktype = -1;
//            board[block.blockpos-1+i].blockvalue = -1;
        }
    }
    //look for the upper
    int upperpos = 0;
    for (int i=block.blockpos-1-COLOMN;i>=col-1;i-=COLOMN)
    {
        if (board[i].blocktype == block.blocktype)
        {
            upperpos++;
            isvalid++;
        }
        else
            break;
    }
    //look for the lower
    int lowerpos = 0;
    for (int i=block.blockpos-1+COLOMN;i<=(ROW-1)*COLOMN+col-1;i+=COLOMN)
    {
        if (board[i].blocktype == block.blocktype)
        {
            lowerpos++;
            isvalid++;
        }
        else
            break;
    }
    if (upperpos + lowerpos >= 2)
    {
        for (int i=0;i<=lowerpos;i++)
        {
            Effects(board, ROW, COLOMN, Warrior, Due, block);
            board[block.blockpos-1+COLOMN*i].istocan = 1;
//            board[block.blockpos-1+COLOMN*i].blocktype = -1;
//            board[block.blockpos-1+COLOMN*i].blockvalue = -1;
        }
        for (int i=0;i<=upperpos;i++)
        {
            Effects(board, ROW, COLOMN, Warrior, Due, block);
            board[block.blockpos-1-COLOMN*i].istocan = 1;
//            board[block.blockpos-1-COLOMN*i].blocktype = -1;
//            board[block.blockpos-1+COLOMN*i].blockvalue = -1;
        }
    }
    //tocan is a parameter to tell how many blocks are cancelled at a time
    tocan = rightpos + leftpos + upperpos + lowerpos;
}

void Cancellation(int ROW,int COLOMN,vector<BLOCK>&board,WARRIOR &Warrior,DUE &Due)
{
    int anycancel;
    while (1)
    {
        anycancel = 0;
        for (int i = 0; i < ROW*COLOMN; i++)
            board[i].isjustmoved = 0;
        for (int i = 0; i < ROW*COLOMN; i++)
            if (board[i].ismoved > 0)
            {
                FindCancellation(board[i], board, ROW, COLOMN, Warrior, Due);
                anycancel++;
            }
        for (int i = 0; i < ROW*COLOMN; i++)
            if (board[i].istocan == 1)
            {
                board[i].blockvalue = -1;
                board[i].blocktype = -1;
                board[i].istocan = 0;
            }
        Draw(ROW, COLOMN, board, Warrior, Due);
        sleep(2);
        Fall(board, ROW, COLOMN, Warrior, Due);
        for (int i = 0; i < ROW*COLOMN; i++)
            if (board[i].isjustmoved == 0)
                board[i].ismoved = 0;
        if (anycancel == 0)
            break;
    }
}

void Swap(string usermove,vector<BLOCK>&board, int ROW, int COLOMN,WARRIOR &warrior,DUE &due)
{
    vector<int> position;
    for (int i=0;usermove[i]!='\0';i++)
        if (usermove[i]==40)
            position.push_back(i);
    string block1row, block1col, block2row, block2col;
    int ii;
    for (ii=1; usermove[position[0]+ii]!=44;ii++)
        block1row += usermove[position[0]+ii];
    int block1r = stod(block1row);
    for (ii++; usermove[position[0]+ii]!=41;ii++)
        block1col += usermove[position[0]+ii];
    int block1c = stod(block1col);
    for (ii=1; usermove[position[1]+ii]!=44;ii++)
        block2row += usermove[position[1]+ii];
    int block2r = stod(block2row);
    for (ii++; usermove[position[1]+ii]!=41;ii++)
        block2col += usermove[position[1]+ii];
    int block2c = stod(block2col);
    int temp1,temp2;
    temp2 = board[(block2r-1) * COLOMN + block2c - 1].blockpos;
    temp1 = board[(block1r-1) * COLOMN + block1c - 1].blockpos;
    board[(block1r-1) * COLOMN + block1c - 1].blockpos = temp2;
    board[(block2r-1) * COLOMN + block2c - 1].blockpos = temp1;
    BLOCK tem1,tem2;
    tem1 = board[(block1r-1) * COLOMN + block1c - 1];
    tem2 = board[(block2r-1) * COLOMN + block2c - 1];
    board[(block1r-1) * COLOMN + block1c - 1] = tem2;
    board[(block2r-1) * COLOMN + block2c - 1] = tem1;
    //set the blocks "ismoved"
    board[(block1r-1) * COLOMN + block1c - 1].ismoved++;
    board[(block1r-1) * COLOMN + block1c - 1].isjustmoved = 1;
    board[(block2r-1) * COLOMN + block2c - 1].ismoved++;
    board[(block2r-1) * COLOMN + block2c - 1].isjustmoved = 1;
    for (int i = 0; i < ROW*COLOMN; i++)
        if (i != (block1r-1) * COLOMN + block1c - 1 && i != (block2r-1) * COLOMN + block2c - 1)
        {
            board[i].ismoved = 0;
            board[i].isjustmoved = 0;
        }
}

void rework(int ROW,int COLOMN,vector<BLOCK>&board,WARRIOR &warrior)
{
    for (int i = 0; i < ROW*COLOMN; i++)
    {
        board[i].blockvalue = rand() % 100 + 1;
        board[i].blocktype = Blocktype(board[i].blockvalue);
        board[i].ismoved = 0;
        board[i].isjustmoved = 0;
    }
    warrior.WarMana -= 30;
}

void MO(string usermove,vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &warrior,DUE &due)
{
    string blockrow = "", blockcol = "";
    int blockr = 0, blockc = 0;
    for (int i = 0; usermove[i]!='\0'; i++)
        if (usermove[i]==40)
        {
            int ii;
            for (ii=1; usermove[i+ii]!=44; ii++)
                blockrow += usermove[i+ii];
            blockr = stod(blockrow);
            for (ii++; usermove[i+ii]!=41; ii++)
                blockcol +=usermove[i+ii];
            blockc = stod(blockcol);
            break;
        }
    string moblock = "";
    for (int i=0; usermove[i]!='\0'; i++)
        if (usermove[i]==41)
            for (int ii=1; usermove[i+ii]!='\0'; ii++)
                if (usermove[i+ii]!=' ')
                    moblock += usermove[i+ii];
    board[(blockr-1)*COLOMN+blockc-1].ismoved++;
    board[(blockr-1)*COLOMN+blockc-1].isjustmoved = 1;
    if (moblock == "1")
    {
        board[(blockr-1)*COLOMN+blockc-1].blocktype = 1;
        board[(blockr-1)*COLOMN+blockc-1].blockvalue = rand() % 31 + 1;
    }
    else if (moblock == "0")
    {
        board[(blockr-1)*COLOMN+blockc-1].blocktype = 2;
        board[(blockr-1)*COLOMN+blockc-1].blockvalue = rand() % 32 + 32;
    }
    else if (moblock == "*")
    {
        board[(blockr-1)*COLOMN+blockc-1].blocktype = 3;
        board[(blockr-1)*COLOMN+blockc-1].blockvalue = rand() % 32 + 64;
    }
    warrior.WarMana -= 15;
    for (int i = 0; i < ROW*COLOMN; i++)
        if (i != (blockr-1)*COLOMN+blockc-1)
        {
            board[i].ismoved = 0;
            board[i].isjustmoved = 0;
        }
}

void putoff(string usermove,vector<BLOCK>&board,int ROW,int COLOMN,WARRIOR &warrior,DUE &due)
{
    string blockrow = "", blockcol = "";
    int blockr = 0, blockc = 0;
    for (int i = 0; usermove[i]!='\0'; i++)
        if (usermove[i]==40)
        {
            int ii;
            for (ii=1; usermove[i+ii]!=44; ii++)
                blockrow += usermove[i+ii];
            blockr = stod(blockrow);
            for (ii++; usermove[i+ii]!=41; ii++)
                blockcol +=usermove[i+ii];
            blockc = stod(blockcol);
            break;
        }
    string putoffblock = "";
    for (int i=0; usermove[i]!='\0'; i++)
        if (usermove[i]==41)
            for (int ii=1; usermove[i+ii]!='\0'; ii++)
                if (usermove[i+ii]!=' ')
                    putoffblock += usermove[i+ii];
    if (putoffblock == "above")
    {
        board[(blockr-2)*COLOMN+blockc-1].blocktype=-1;
        board[(blockr-2)*COLOMN+blockc-1].isjustmoved = 1;
        board[(blockr-2)*COLOMN+blockc-1].ismoved++;
        board[(blockr-1)*COLOMN+blockc-2].blocktype=-1;
        board[(blockr-1)*COLOMN+blockc-2].isjustmoved = 1;
        board[(blockr-1)*COLOMN+blockc-2].ismoved++;
        board[(blockr)*COLOMN+blockc-1].blocktype=-1;
        board[(blockr)*COLOMN+blockc-1].isjustmoved = 1;
        board[(blockr)*COLOMN+blockc-1].ismoved++;
        //        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
        cout<<board[(blockr-1)*COLOMN+blockc-1].blocktype<<endl;
        //insert cancellation
        Cancellation(ROW, COLOMN, board, warrior, due);
    }
    if (putoffblock == "below")
    {
        board[(blockr-2)*COLOMN+blockc-1].blocktype=-1;
        board[(blockr-2)*COLOMN+blockc-1].ismoved++;
        board[(blockr-2)*COLOMN+blockc-1].isjustmoved = 1;
        board[(blockr)*COLOMN+blockc-1].blocktype=-1;
        board[(blockr)*COLOMN+blockc-1].isjustmoved = 1;
        board[(blockr)*COLOMN+blockc-1].ismoved++;
        board[(blockr-1)*COLOMN+blockc].blocktype=-1;
        board[(blockr-1)*COLOMN+blockc].ismoved++;
        board[(blockr-1)*COLOMN+blockc].isjustmoved = 1;
        //        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
        cout<<board[(blockr-1)*COLOMN+blockc-1].blocktype<<endl;
        //insert cancellation
        Cancellation(ROW, COLOMN, board, warrior, due);
    }
    if (putoffblock == "left")
    {
        board[(blockr-1)*COLOMN+blockc].blocktype=-1;
        board[(blockr-1)*COLOMN+blockc].isjustmoved = 1;
        board[(blockr-1)*COLOMN+blockc].ismoved++;
        board[(blockr-1)*COLOMN+blockc-2].blocktype=-1;
        board[(blockr-1)*COLOMN+blockc-2].ismoved++;
        board[(blockr-1)*COLOMN+blockc-2].isjustmoved = 1;
        board[(blockr)*COLOMN+blockc-1].blocktype=-1;
        board[(blockr)*COLOMN+blockc-1].ismoved++;
        board[(blockr)*COLOMN+blockc-1].isjustmoved = 1;
        //        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
        cout<<board[(blockr-1)*COLOMN+blockc-1].blocktype<<endl;
        //insert cancellation
        Cancellation(ROW, COLOMN, board, warrior, due);
    }
    if (putoffblock == "right")
    {
        board[(blockr-2)*COLOMN+blockc-1].blocktype=-1;
        board[(blockr-2)*COLOMN+blockc-1].isjustmoved = 1;
        board[(blockr-2)*COLOMN+blockc-1].ismoved++;
        board[(blockr-1)*COLOMN+blockc].blocktype=-1;
        board[(blockr-1)*COLOMN+blockc].ismoved++;
        board[(blockr-1)*COLOMN+blockc].isjustmoved = 1;
        board[(blockr-1)*COLOMN+blockc-2].blocktype=-1;
        board[(blockr-1)*COLOMN+blockc-2].ismoved++;
        board[(blockr-1)*COLOMN+blockc-2].isjustmoved = 1;
        //        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
        cout<<board[(blockr-1)*COLOMN+blockc-1].blocktype<<endl;
        //insert cancellation
        Fall(board, ROW, COLOMN, warrior, due);
        Cancellation(ROW, COLOMN, board, warrior, due);
    }
    
}

void savegame(int ROW, int COLOMN, WARRIOR &Warrior, DUE &Due, vector<BLOCK>&board)
{
    fstream file;
    int issave = 1;
    string name;
    cout << "Please input the filename to save into: ";
    getline(cin,name);
    for (int i = 0; i < name.length(); i++)
    {
        if ((name[i]>'z' || name[i]<'a')&&(name[i]>'Z' || name[i]<'A')&&name[i]!='_')
        {
            issave = 0;
            isvalid = 0;
        }
    }
    if (issave == 1)
    {
        file.open(name+".txt",ios::out);
        file << ROUND << '\n' << ROW << '\n' << COLOMN << '\n' << Due.DueArmor<< '\n'
        << Warrior.WarHealth << '\n' << Warrior.WarArmor << '\n' << Warrior.WarMana<< '\n';
        for (int i = 0; i < board.size(); i++)
            file << board[i].blocktype << '\n' << board[i].blockvalue << '\n' << board[i].ismoved
            << '\n' << board[i].isjustmoved << '\n';
        file.close();
        cout << "Done! " << endl;
        char c;
        while (c == '\0')
        {
            cout << "Press enter to continue... " << endl;
            c = getchar();
        }
    }
}

void loadgame(int ROW, int COLOMN, WARRIOR &Warrior, DUE &Due, vector<BLOCK>&board)
{
    fstream file;
    string name;
    int tempmemory;
    BLOCK tempmemo;
    cout << "Please input the filename to load from: ";
    getline(cin,name);
    file.open(name + ".txt",ios::in);
    vector<int> memory;
    int data;
    while( ! file.eof() )
    {
        file >> data;
        memory.push_back(data);
    }
    
    if (memory.size() != memory[1]*memory[2]*4+8)
        isvalid = -2;
    else
    {
        file.close();
        file.open(name+".txt",ios::in);
        file >> ROUND >> ROW >> COLOMN >> Due.DueArmor
        >> Warrior.WarHealth >> Warrior.WarArmor >> Warrior.WarMana;
        unsigned long size = board.size();
        for (int i = 0; i < size; i++)
            board.pop_back();
        for (int i = 0; i < ROW*COLOMN; i++)
        {
            file >> tempmemory;
            tempmemo.blocktype = tempmemory;
            file >> tempmemory;
            tempmemo.blockvalue = tempmemory;
            file >> tempmemory;
            tempmemo.ismoved = tempmemory;
            file >> tempmemory;
            tempmemo.isjustmoved = tempmemory;
            board.push_back(tempmemo);
        }
        file.close();
        cout << "Done! " << endl;
        char c;
        while (c == '\0')
        {
            cout << "Press enter to continue... " << endl;
            c = getchar();
        }
    }
}

void Set(int &ROW,int &COLOMN,DUE &due,WARRIOR &warrior)
{
    int iscorrect = 0;
    string parameter = "";
    cout << "Input name of parameter you want to set: ";
    getline(cin,parameter);
    if (parameter == "Row" || parameter == "row")
    {
        iscorrect++;
        cout << "Please input new row number: ";
        cin >> ROW;
    }
    else if (parameter == "Colomn" || parameter == "colomn")
    {
        iscorrect++;
        cout << "Please input new colomn number: ";
        cin >> COLOMN;
    }
    else if (parameter == "DueArmor" || parameter == "Duearmor" || parameter == "duearmor")
    {
        iscorrect++;
        int Duearmor;
        cout << "Please input new "+parameter << ": ";
        cin >> Duearmor;
    }
    else if (parameter == "WarriorHealth" || parameter == "Warriorhealth" || parameter == "warriorhealth")
    {
        iscorrect++;
        ishealth = 1;
        int WarriorHealth;
        cout << "Please input new "+parameter << ": ";
        cin >> WarriorHealth;
    }
    else if (parameter == "WarriorArmor" || parameter == "Warriorarmor" || parameter == "warriorarmor")
    {
        iscorrect++;
        isarmor = 1;
        int WarriorArmor;
        cout << "Please input new "+parameter << ": ";
        cin >> WarriorArmor;
    }
    else if (parameter == "WarriorMana" || parameter == "Warriormana" || parameter == "warriormana")
    {
        iscorrect++;
        ismana = 1;
        int WarriorMana;
        cout << "Please input new "+parameter << ": ";
        cin >> WarriorMana;
    }
    if (iscorrect > 0)
    {
        cout << "Done! " << endl;
        char c;
        while (c == '\0')
        {
            cout << "Press enter to restart... " << endl;
            c = getchar();
        }
        isrestart = 1;
    }
    else if (iscorrect == 0)
        isvalid = 0;
}

void Help()
{
    cout << "no help" << endl;
    char c;
    while (c == '\0')
    {
        cout << "Press enter to continue... " << endl;
        c = getchar();
    }
}

void isWin(DUE &due, WARRIOR &warrior)
{
    if (iswin != 2)
    {
        if (due.DueArmor <= 0)
            iswin = 1;
        else if (warrior.WarHealth <= 0)
            iswin = -1;
    }
}

void isValid(string usermove,vector<BLOCK>&board,int &ROW,int &COLOMN,WARRIOR &warrior,DUE &due)
{
    isvalid = 0;
    //swap  prototype: swap (x1,y1) (x2,y2)
    for (int i=0; usermove[i]!='\0'; i++)
    {
        //find "swap" and "()"
        if ((usermove[i]=='s'||usermove[i]=='S')&&(usermove[i+1]=='w'||usermove[i+1]=='W')
            &&(usermove[i+2]=='a'||usermove[i+2]=='A')&&(usermove[i+3]=='p'||usermove[i+3]=='P'))
            //have found i, looking for "()"
        {
            int ii;
            if (usermove[i+4]==40)
                isvalid++;
            for (ii=4;usermove[i+ii]!=40;ii++)  //looking for "(", all space before "("
                if (usermove[i+ii] != ' ')
                {
                    isvalid = 0;
                    break;
                }
                else if (usermove[i+ii]==' ')
                    isvalid++;
            if (isvalid > 0)
                for (ii++;usermove[i+ii]!=44;ii++)  //looking for ","
                    if (usermove[i+ii] >= '0' && usermove[i+ii] <= '9')
                        isvalid++;
                    else
                    {
                        isvalid = 0;
                        break;
                    }
                    else break;
            if (isvalid > 0)
                for (ii++;usermove[i+ii]!=41;ii++)  //looking for ")"
                    if (usermove[i+ii] >= '0' && usermove[i+ii] <= '9')
                        isvalid++;
                    else
                    {
                        isvalid = 0;
                        break;
                    }
                    else break;
            if (usermove[i+ii+1]!=40)
            {
                if (isvalid > 0)
                    for (ii++;usermove[i+ii]!=40;ii++)  //looking for the second "("
                        if (usermove[i+ii] != ' ')
                        {
                            isvalid = 0;
                            break;
                        }
                        else
                            isvalid++;
                        else break;
            }
            else if (usermove[i+ii+1]==40)
                ii++;
            if (isvalid > 0)
                for (ii++;usermove[i+ii]!=44;ii++)
                    if (usermove[i+ii] >= '0' && usermove[i+ii] <= '9')
                        isvalid++;
                    else
                    {
                        isvalid = 0;
                        break;
                    }
                    else break;
            if (isvalid > 0)
                for (ii++;usermove[i+ii]!=41;ii++)
                    if (usermove[i+ii] >= '0' && usermove[i+ii] <= '9')
                        isvalid++;
                    else
                    {
                        isvalid = 0;
                        break;
                    }
                    else break;
            for (int iii=0; iii<i; iii++)
                if (usermove[iii]!=' ')
                    isvalid=0;
            if (isvalid > 0)
            {
                Swap(usermove, board, ROW, COLOMN,warrior,due);
                break;
            }
        }
    }
    //mo    prototype: mo (x,y) value
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        if ((usermove[i]=='m' || usermove[i]=='M') && (usermove[i+1]=='o'||usermove[i+1]=='O'))
        {
            int ii;
            if (usermove[i+2]==40)
                isvalid++;
            for (ii=2;usermove[i+ii]!=40;ii++)
            {
                if (usermove[i+ii]==' ')
                    isvalid++;
                else if (usermove[i+ii]!=' ')
                {
                    isvalid = 0;
                    break;
                }
            }
            if (isvalid > 0)
            {
                for (ii++;usermove[i+ii]!=44;ii++)
                    if (usermove[i+ii] >= '0' && usermove[i+ii] <= '9')
                        isvalid++;
                    else
                    {
                        isvalid = 0;
                        break;
                    }
            }
            if (isvalid > 0)
            {
                for (ii++;usermove[i+ii]!=41;ii++)
                    if (usermove[i+ii] < '0' || usermove[i+ii] > '9')
                    {
                        isvalid = 0;
                        break;
                    }
                    else isvalid++;
            }
            if ((usermove[i+ii+1]<'0' || usermove[i+ii+1] > '9') && usermove[i+ii+1]!='*')
            {
                if (isvalid > 0)
                {
                    for (ii++;(usermove[i+ii]<'0'||usermove[i+ii]>'9')&& usermove[i+ii]!='*';ii++)
                        if (usermove[i+ii]!=' ')
                        {
                            isvalid = 0;
                            break;
                        }
                        else isvalid++;
                }
            }
            else ii++;
            if (isvalid > 0)
            {
                for (ii++;usermove[i+ii]!='\0';ii++)
                    if (usermove[i+ii]=='0' || usermove[i+ii]=='1' || usermove[i+ii]=='*')
                        isvalid++;
                    else {
                        isvalid = 0;
                        break;
                    }
            }
            for (int iii=0; iii<i; iii++)
                if (usermove[iii]!=' ')
                    isvalid=0;
            if (isvalid > 0 && warrior.WarMana >= 15)
            {
                MO(usermove, board, ROW, COLOMN, warrior,due);
                break;
            }
            if (warrior.WarMana < 15)
            {
                isvalid = 0;
                break;
            }
        }
    }
    //putoff    prototype: putoff(x,y) direction
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        if (usermove[i]=='p'&&usermove[i+1]=='u'&&usermove[i+2]=='t'&&usermove[i+3]=='o'&&usermove[i+4]=='f'
            &&usermove[i+5]=='f')
        {
            int ii;
            if (usermove[i+6]==40)
                isvalid++;
            for (ii=6;usermove[i+ii]!=40;ii++)
            {
                if (usermove[i+ii]==' ')
                    isvalid++;
                else if (usermove[i+ii]!=' ')
                {
                    isvalid = 0;
                    break;
                }
            }
            if (isvalid > 0)
            {
                for (ii++;usermove[i+ii]!=44;ii++)
                    if (usermove[i+ii] >= '0' && usermove[i+ii] <= '9')
                        isvalid++;
                    else
                    {
                        isvalid = 0;
                        break;
                    }
            }
            if (isvalid > 0)
            {
                for (ii++;usermove[i+ii]!=41;ii++)
                    if (usermove[i+ii] < '0' || usermove[i+ii] > '9')
                    {
                        isvalid = 0;
                        break;
                    }
                    else isvalid++;
            }
            for (int iii=0; iii<i; iii++)
                if (usermove[iii]!=' ')
                    isvalid=0;
            if (isvalid > 0 && warrior.WarMana >= 15)
            {
                putoff(usermove, board, ROW, COLOMN, warrior, due);
                break;
            }
        }
    }
    //save      prototype: save
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        if ((usermove[i]=='s'||usermove[i]=='S')&&(usermove[i+1]=='a'||usermove[i]=='A')
            &&(usermove[i+2]=='v'||usermove[i+2]=='V')&&(usermove[i+3]=='e'||usermove[i+3]=='E')
            &&usermove[i+4]=='\0')
        {
            isvalid++;
            for (int ii = 0; ii < i; ii++)
                if (usermove[ii]!=' ')
                {
                    isvalid = 0;
                    break;
                }
            if (isvalid > 0)
            {
                savegame(ROW, COLOMN, warrior, due, board);
                break;
            }
        }
    }
    //load      prototype: load
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        if ((usermove[i]=='l'||usermove[i]=='L')&&(usermove[i+1]=='o'||usermove[i]=='O')
            &&(usermove[i+2]=='a'||usermove[i+2]=='A')&&(usermove[i+3]=='d'||usermove[i+3]=='D')
            &&usermove[i+4]=='\0')
        {
            isvalid++;
            for (int ii = 0; ii < i; ii++)
                if (usermove[ii]!=' ')
                {
                    isvalid = 0;
                    break;
                }
            if (isvalid > 0)
            {
                loadgame(ROW, COLOMN, warrior, due, board);
                if (isvalid == -2)
                {
                    cout << "The datas have been damaged! " << endl;
                    char c;
                    while (c == '\0')
                    {
                        cout << "Press enter to continue present game... " << endl;
                        c = getchar();
                    }
                }
                break;
            }
        }
    }
    //rework    prototype: rework
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        if ((usermove[i]=='r'||usermove[i]=='R')&&(usermove[i+1]=='e'||usermove[i+1]=='E')
            &&(usermove[i+2]=='w'||usermove[i+2]=='W')&&(usermove[i+3]=='o'||usermove[i+3]=='O')
            &&(usermove[i+4]=='r'||usermove[i+4]=='R')&&(usermove[i+5]=='k'||usermove[i+5]=='K'))
        {
            isvalid++;
            if (usermove[i+6]!='\0')
                isvalid = 0;
            for (int ii = 0; ii<i; ii++)
                if (usermove[ii]!=' ')
                {
                    isvalid = 0;
                    break;
                }
            if (isvalid > 0 && warrior.WarMana >= 30)
            {
                rework(ROW, COLOMN, board, warrior);
                break;
            }
            if (warrior.WarMana < 30)
            {
                isvalid = 0;
                break;
            }
        }
    }
    //greedisgood
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        string isgreed = "", greednum = "";
        int greed = 0;
        if (usermove[i]!=' ')
        {
            int ii;
            for (ii = 0; usermove[i+ii] != ' '; ii++)
                isgreed += usermove[i+ii];
            if (isgreed == "greedisgood")
            {
                for (; usermove[i+ii] != ' '; ii++);
                for (; usermove[i+ii] != '\0'; ii++)
                    greednum += usermove[i+ii];
                if (greednum.length() < 11)
                {
                    greed = stod(greednum);
                    isvalid++;
                }
                else isvalid = 0;
                warrior.WarMana += greed;
                cout << "You have added " << greed << " manas." << endl;
            }
            break;
        }
    }
    //terribleterribledamage
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        string terrible = "";
        if (usermove[i]!=' ')
        {
            for (int ii=0; usermove[i+ii]!='\0'; ii++)
                terrible += usermove[i+ii];
            if (terrible == "terribleterribledamage")
            {
                warrior.WarArmor += 20;
                isvalid++;
                cout << "You have added 20 armors." <<endl;
            }
            break;
        }
    }
    //Ilovelab
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        string shortcut = "";
        if (usermove[i]!=' ')
        {
            for (int ii=0; usermove[i+ii]!='\0'; ii++)
                shortcut += usermove[i+ii];
        }
        if (shortcut == "Ilovelab" || shortcut == "Ilovevg101")
        {
            iswin = 2;
            isvalid++;
            break;
        }
    }
    //set protype: set
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        string isset = "";
        int ii;
        for (ii = 0; usermove[ii] == ' '; ii++);
        for (; usermove[ii]!='\0'; ii++)
            isset += usermove[ii];
        if (isset == "set")
        {
            isvalid++;
            Set(ROW, COLOMN, due, warrior);
            while (isvalid == 0)
            {
                Set(ROW, COLOMN, due, warrior);
            }
            if (isvalid > 0)
                break;
        }
    }
    //help page
    for (int i = 0; usermove[i]!='\0'; i++)
    {
        string ishelp = "";
        if (usermove[i]!=' ')
        {
            for (int ii=0; usermove[i+ii]!='\0'; ii++)
                ishelp += usermove[i+ii];
        }
        if (ishelp == "help")
        {
            Help();
            isvalid --;
        }
    }
    if (isrestart == 0)
        Cancellation(ROW, COLOMN, board, warrior, due);
}

int main()
{
    BOARD Board(8,8);
    int ROW = Board.Row;
    int COLOMN = Board.Colomn;
    srand(unsigned (time(NULL)));
    vector<BLOCK> board;
    //generate the randoms
    for (int i = 0; i < ROW*COLOMN; i++)
    {
        BLOCK block;
        block.blockvalue = 1;
        block.blocktype = Blocktype(block.blockvalue);
        block.blockpos = i+1;
        block.ismoved = 0;
        block.isjustmoved = 0;
        block.istocan = 0;
        board.push_back(block);
    }
    WARRIOR Warrior(8,1);
    DUE Due(60,1);
    while (1)
    {
        if (isrestart > 0)
        {
            BOARD Board(ROW, COLOMN);
            ROW = Board.Row;
            COLOMN = Board.Colomn;
            board.clear();
            for (int i = 0; i < ROW*COLOMN; i++)
            {
                BLOCK block;
                block.blockvalue = rand() % 100 + 1;
                block.blocktype = Blocktype(block.blockvalue);
                block.blockpos = i+1;
                block.ismoved = 0;
                block.isjustmoved = 0;
                block.istocan = 0;
                board.push_back(block);
            }
            ROUND = 1;
            DUE Due(Duearmor,1);
            if (ishealth == 1)
                WARRIOR Warrior(WarriorHealth,1);
            else if (isarmor == 1)
                WARRIOR Warrior(WarriorArmor,1,2);
            else if (ismana == 1)
                WARRIOR Warrior(WarriorMana,'a');
        }
        isWin(Due,Warrior);
        if (iswin == 0)
        {
            Draw(ROW, COLOMN, board, Warrior, Due);
            cout << "> ";
            string usermove = "";
            getline(cin,usermove);
            isValid(usermove, board, ROW, COLOMN, Warrior, Due);
            while (isvalid <=  0)
            {
                if (isvalid == 0)
                    cout << "Unknown command! " << endl
                    << "Enter help to see all valid commands or enter commands again."
                    << endl << "> ";
                else if (isvalid == -1)
                {
                    Draw(ROW, COLOMN, board, Warrior, Due);
                    cout << "> ";
                    usermove = "";
                }
                getline(cin,usermove);
                isValid(usermove, board, ROW, COLOMN, Warrior, Due);
            }
            ROUND+=2;
        }
        else if (iswin == -1)
        {
            cout << "Warrior is defeated by Due! " << endl
            << "You lose! " << endl;
            break;
        }
        else if (iswin == 1)
        {
            cout << "Due is defeated by Warrior!" << endl
            << "You win! " << endl;
            break;
        }
        else if (iswin == 2)
        {
            cout << "You win directly! " << endl;
            break;
        }
        Duemove(ROW, COLOMN, Warrior, Due, board);
    }
    return 0;
}




















