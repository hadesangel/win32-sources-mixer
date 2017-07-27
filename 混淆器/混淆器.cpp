// 混淆器.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <fstream>
#include "InstructionMixer.h"
#include <iostream>
using namespace std;

#define HARDNESS_LOW 3
#define HARDNESS_HIG 64
int main(int argc, char* argv[])
{
	char srcFilePath[ 260 ];
	char desFilePath[ 260 ];
	int hardness = HARDNESS_LOW;

	if( argc < 3 ) {
		//cout << "命令行格式： 源汇编文件路径 输出文件路径" << endl;
		cout << "输入源文件路径:";
		cin.getline( srcFilePath,260 );
		cout << "输入目标文件路径:";
		cin.getline( desFilePath , 260 );

		argv[ 1 ] = srcFilePath;
		argv[ 2 ] = desFilePath;
		cout << "输入混淆级别1~63:";
		cin >> hardness;
	}

	if (argc == 4)
	{
		hardness = atol(argv[3]);
		if (hardness <= 0)
			hardness = HARDNESS_LOW;
		if (hardness > 64)
			hardness = HARDNESS_HIG;
	}

	Instruction::initInstructionLibrary();

	// 混淆引擎1
	MixEngine_bit mix_bit;
	mix_bit.setHardness(hardness);
	
	MixEngine_tran mix_tran;
	mix_tran.setHardness(hardness);

	// 混淆引擎2
	MixEngine_jcc mix_jcc;
	mix_jcc.setHardness(hardness);

	// 混淆器
	InstructionMixer mixer;
	

	// 将混淆引擎添加进混淆器中
	mixer.addEngine( &mix_tran );
	mixer.addEngine( &mix_bit );
	mixer.addEngine( &mix_jcc );
	
	// 打开和创建文件
	ofstream oFile( argv[ 2 ]  );
	ifstream iFile( argv[1] , ios::in  );

	if( iFile.is_open( )==false) {
		cout << argv[ 1 ] << "文件无法打开" << endl;
		return 0;
	}
	if( oFile.is_open( ) == false) {
		cout << argv[ 2 ] << "文件无法打开" << endl;
		return 0;
	}


	char line[200];
	vector<Instruction> vecIns;
	
	// 获取每一行汇编源文件的代码
	while( iFile.getline(line,200) ) {

		// 混淆
 		if( mixer.mix( line , &vecIns ) ) {
			// 混淆成功，则将混淆后的内容输出到文件
			for( auto &i : vecIns ) {
				oFile << i << endl;
			}
		}
		else {
			// 混淆失败则将原指令输出到文件。
			oFile << line << endl;
		}
	}


	oFile.close( );
	iFile.close( );
	return 0;
}

