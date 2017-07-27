#pragma once
#include "Instruction.h"
#include <vector>
#include "MixEngine.h"
using std::vector;

class InstructionMixer {

	vector<MixEngine*> m_engine;  // 混合引擎
public:

	//开始混淆
	/*!
	 * \brief mix
	 * 混淆输入进来的指令, 将生成的混淆指令输出到vector中.
	 * 在混淆时,如果遇到MACRO 宏,则不做处理,返回false
	 * 如果输入进来的指令是一条未被分类的指令(e_none),则不做混淆,函数直接返回false
	 * 如果指令是被分类的. 则循环调用所有混淆引擎开始进行混淆.
	 * 在调用混淆引擎的过程中,如果一个引擎成功进行了混淆,则后续的混淆引擎将没有处理的机会.
	 * 函数直接返回true.
	 * \ret bool 返回false表示混淆失败.true为混淆成功
	 * \param const char * pIns 待混淆的原指令
	 * \param vector<Instruction> * vecMixer 混淆后的指令,包含原指令
	 */
	bool mix( const char* pIns , vector<Instruction>* vecMixer );

	
	/*!
	 * \brief addEngine
	 * 添加混淆引擎
	 * \param MixEngine * pEngine
	 */
	void addEngine( MixEngine* pEngine );
};

