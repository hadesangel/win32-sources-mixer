#pragma once
#include <map>
#include <vector>
using std::map;

enum INSTRUCTION_CLASS 
{
	e_none,
	e_tran,// 数据传输指令,类似mov，add，push，xchg，pop，
	e_bit, // 位操作指令
	e_jcc, // 所有条件跳转指令
	e_jmp, // 直接跳转指令
	e_call,// 函数调用指令 有call offset , call [address]
	e_str  // 串操作指令
};


#define MAX_OPERATORSTRING	64
#define MAX_INS				64
#define MAX_OPERAOTCOUNT	4


unsigned int hashCode( const char* pStr );

class TString;
class InstructionGroup;

class Instruction {

private:
	INSTRUCTION_CLASS	m_insClass;
	int			m_operatorCount;    // 指令操作数个数
	char		m_mnemonic[ MAX_INS ]; // 指令
	char		m_operator[MAX_OPERAOTCOUNT][MAX_OPERATORSTRING]; // 操作数

public:
	friend InstructionGroup;
	Instruction( );
	explicit Instruction( const char* pIns);

	bool		setInstruction( const char* pIns );

	// 指令类型
	
	INSTRUCTION_CLASS	type()const;
	TString		instruction( )const;
	const char* mnemonic( )const;
	const char* operator1()const;
	const char* operator2( )const;
	const char* operator3( )const;

	void		setMnemonic( const char* mnemonic );
	void		setOperator1(const char* pOperator );
	void		setOperator2( const char* pOperator );
	void		setOperator3( const char* pOperator );

	friend std::ostream& operator<<(std::ostream& o, const Instruction& ins);
	friend std::ofstream& operator<<(std::ofstream& o, const Instruction& ins);


	static bool addInstruct( const char* pIns , INSTRUCTION_CLASS type );
	static bool	initInstructionLibrary( );
	static INSTRUCTION_CLASS checkInstructClass(const char* mnemonic);
private:
protected:
	static bool m_bInit;
	static map<unsigned int, INSTRUCTION_CLASS> m_insList; // 指令-类型映射
};

class InstructionGroup : public std::vector<Instruction>
{
public:
	void addInstruction(const Instruction& ins);
	
	TString toString()const;
	InstructionGroup() = default;
	InstructionGroup(const char* pIns);
	InstructionGroup(const TString& pIns);
};