#pragma once
#include <string>
#include <vector>
#include <list>

#define POLY_MAXVALUE	0
#define POLY_NONE		POLY_MAXVALUE
#define POLY_ROOT		POLY_MAXVALUE + 1
#define POLY_MUL		POLY_MAXVALUE + 2
#define POLY_PLU		POLY_MAXVALUE + 3
#define POLY_POW		POLY_MAXVALUE + 4
#define POLY_MIN		POLY_MAXVALUE + 5
#define POLY_DIV		POLY_MAXVALUE + 6
#define POLY_OPEN		POLY_MAXVALUE + 7
#define POLY_CLOSE		POLY_MAXVALUE + 8
#define POLY_NUM		POLY_MAXVALUE + 9
#define POLY_ID			POLY_MAXVALUE + 10
#define POLY_EOS		POLY_MAXVALUE + 11
#define POLY_COS		POLY_MAXVALUE + 12
#define POLY_SIN		POLY_MAXVALUE + 13
#define POLY_TAN		POLY_MAXVALUE + 14
#define POLY_COSEC		POLY_MAXVALUE + 15
#define POLY_CSC		POLY_COSEC
#define POLY_SEC		POLY_MAXVALUE + 16
#define POLY_COT		POLY_MAXVALUE + 17
#define POLY_PI			POLY_ID
#define POLY_EXP		POLY_ID
#define POLY_LOG		POLY_MAXVALUE + 18
#define POLY_LN			POLY_MAXVALUE + 19
#define POLY_LOG10		POLY_MAXVALUE + 20

#define POLY_ABS		POLY_MAXVALUE + 21
#define POLY_MINF		POLY_MAXVALUE + 22
#define POLY_MAXF		POLY_MAXVALUE + 23
#define POLY_IRAND		POLY_MAXVALUE + 24
#define POLY_FRAND		POLY_MAXVALUE + 25
#define POLY_MOD		POLY_MAXVALUE + 26
#define POLY_FLOOR		POLY_MAXVALUE + 27
#define POLY_SIGN		POLY_MAXVALUE + 28

#define POLY_MAXSTACK	100

#define MID_UNKNOWN		0
#define MID_NUMBER		256
#define MID_VARIABLE		512
#define MID_SYMBOL		1024

#define MID_LONG		MID_NUMBER + 1
#define MID_SQRT		MID_NUMBER + 2
#define MID_FRACTION		MID_NUMBER + 3

#define ST_UNKNOWN	0
#define ST_PLUS		11
#define ST_MINUS	12
#define ST_MULTIPLY	23
#define ST_DIVIDE	24
#define ST_CARET	35
#define ST_OPEN		06
#define ST_CLOSE	07

#define SY_PLUS		'+'
#define SY_MINUS	'-'
#define SY_MULTIPLY	'*'
#define SY_DIVIDE	'/'
#define SY_CARET	'^'
#define SY_OPEN		'('
#define SY_CLOSE	')'

struct CBase
{
	CBase();
	virtual ~CBase();

	bool isSymbol();
	bool isVar();
	bool isNumber();

	int id;
};

class CSymbol : public CBase
{
public:
	CSymbol();
	virtual ~CSymbol();

	static int issymbol(int ch);

	void SetType(int Type);
	int GetType();

	bool Equal(CSymbol dif);
	bool Less(CSymbol dif);

private:
	int	iType;
};

struct CSymTable
{
	CSymTable(int aTok, const std::string& aStr);
	virtual ~CSymTable();

	double dVal;
	int token;
	std::string	strlex;
};

class CPoly
{
public:
	enum ERandomType
	{
		RANDOM_TYPE_FREELY,
		RANDOM_TYPE_FORCE_MIN,
		RANDOM_TYPE_FORCE_MAX,
	};

	CPoly();
	virtual ~CPoly();

	int		Analyze(const char* pszStr = NULL);
	double	Eval();

	void SetRandom(int iRandomType);

	void SetStr(const std::string& str);

	int SetVar(const std::string& strName, double dVar);
	double GetVar(const std::string& strName);

	void Clear();

protected:
	int my_irandom(double start, double end);
	double my_frandom(double start, double end);

	void init();
	int insert(const std::string& s, int tok);
	int find(const std::string& s);
	void emit(int t, int tval);
	void match(int t);
	void expo();
	void factor();
	void term();
	void error();
	void expr();
	int lexan();

	int iToken;
	double iNumToken;
	int iLookAhead;
	int iErrorPos;
	bool ErrorOccur;
	unsigned int uiLookPos;

	// NOTE: list is slight faster than vector, why?!
	std::vector<int> tokenBase;
	std::vector<double> numBase;
	std::vector<CSymTable*> lSymbol;
	std::vector<int> SymbolIndex;
	int STSize;
	int MathSymbolCount;
	std::string strData;
	int m_iRandomType;
};
