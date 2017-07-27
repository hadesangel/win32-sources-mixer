#include "stdafx.h"
#include "InstructionMixer.h"
#include "tstring.h"

bool InstructionMixer::mix(const char* pIns , vector<Instruction>* vecMixer ) {

	static bool inMacro = false;
	TString ins(pIns);
	ins = ins.toUpper();

	char *pBuff = ins.data();

	if( inMacro == false && strstr( pBuff , " MACRO " ) != nullptr ) {
		inMacro = true;
		return false;
	}
	else if( inMacro == true && strstr( pBuff , "ENDM" ) != nullptr ) {
		inMacro = false;
		return false;
	}

	// 不处理宏的内容
	if( inMacro )
		return false;


	Instruction insObj( pIns );
	// 如果指令尚未被分类,则不做处理
	if( insObj.type( ) == e_none )
		return false;
	
	for( auto& i : m_engine ) {
		if( i->mix( insObj , vecMixer ) ) {
			return true;
		}
	}

	return false;
}

void InstructionMixer::addEngine( MixEngine* pEngine ) {
	m_engine.push_back( pEngine);
}
