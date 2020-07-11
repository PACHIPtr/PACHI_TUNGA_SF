#include "poly.h"

#include <string>
#include <cmath>
#include <cctype>
#include <cstdlib>

double _random()
{
	return std::rand() / (RAND_MAX + 1.0);
}

CBase::CBase()
{
	id = 0;
}

CBase::~CBase()
{
}

bool CBase::isNumber()
{
	return (id & MID_NUMBER) != 0 ? true : false;
}

bool CBase::isVar()
{
	return (id & MID_VARIABLE) != 0 ? true : false;
}

bool CBase::isSymbol()
{
	return (id & MID_SYMBOL) != 0 ? true : false;
}

CSymbol::CSymbol()
{
	id = MID_SYMBOL;
	iType = ST_UNKNOWN;
}

CSymbol::~CSymbol()
{
}

bool CSymbol::Equal(CSymbol dif)
{
	if (dif.iType / 10 == iType / 10) return true;
	return false;
}

bool CSymbol::Less(CSymbol dif)
{
	if (dif.iType / 10 > iType / 10) return true;
	return false;
}

int CSymbol::GetType()
{
	return iType;
}

void CSymbol::SetType(int Type)
{
	iType = Type;
}

int CSymbol::issymbol(int ch)
{
	switch (ch)
	{
	case SY_PLUS: return ST_PLUS;
	case SY_MINUS: return ST_MINUS;
	case SY_MULTIPLY: return ST_MULTIPLY;
	case SY_DIVIDE: return SY_DIVIDE;
	case SY_CARET: return SY_CARET;
	case SY_OPEN: return SY_OPEN;
	case SY_CLOSE: return ST_CLOSE;
	}

	return 0;
}

CSymTable::CSymTable(int aTok, const std::string & aStr)
	: dVal(0)
	, token(aTok)
	, strlex(aStr)
{
}

CSymTable::~CSymTable()
{
}

CPoly::CPoly()
	: iToken(0)
	, iNumToken(0)
	, iLookAhead(0)
	, iErrorPos(0)
	, ErrorOccur(true)
	, uiLookPos(0)
	, STSize(0)
{
	lSymbol.clear();
	lSymbol.reserve(50);

	init();
}

CPoly::~CPoly()
{
	Clear();
}

void CPoly::SetStr(const std::string & str)
{
	strData = str;
}

double CPoly::Eval()
{
	int stNow;
	double save[POLY_MAXSTACK], t;
	int iSp = 0;

	if (ErrorOccur) {
		return 0;
	}

	auto pos = tokenBase.begin();
	auto posn = numBase.begin();

	while (pos != tokenBase.end()) {
		stNow = *pos;
		++pos;
		switch (stNow) {
		case POLY_NUM:
			save[iSp++] = *posn++;
			break;

		case POLY_ID:
			save[iSp++] = lSymbol[*pos]->dVal;
			pos++;
			break;

		case POLY_PLU:
			iSp--;
			save[iSp - 1] += save[iSp];
			break;

		case POLY_MIN:
			iSp--;
			save[iSp - 1] -= save[iSp];
			break;

		case POLY_MUL:
			iSp--;
			save[iSp - 1] *= save[iSp];
			break;

		case POLY_MOD:
			iSp--;
			if (save[iSp] == 0)
				return 0;

			save[iSp - 1] = fmod(save[iSp - 1], save[iSp]);
			break;

		case POLY_DIV:
			iSp--;
			if (save[iSp] == 0)
				return 0;

			save[iSp - 1] /= save[iSp];
			break;

		case POLY_POW:
			iSp--;
			save[iSp - 1] = pow(save[iSp - 1], save[iSp]);
			break;

		case POLY_ROOT:
			if (save[iSp - 1] < 0)
				return 0;

			save[iSp - 1] = sqrt(save[iSp - 1]);
			break;

		case POLY_COS:
			save[iSp - 1] = cos(save[iSp - 1]);
			break;

		case POLY_SIN:
			save[iSp - 1] = sin(save[iSp - 1]);
			break;

		case POLY_SIGN:
			if (save[iSp - 1] == 0.0)
				save[iSp - 1] = 0.0;
			else if (save[iSp - 1] < 0.0)
				save[iSp - 1] = -1.0;
			else
				save[iSp - 1] = 1.0f;

			break;

		case POLY_TAN:
			if (!(t = cos(save[iSp - 1])))
				return 0;

			save[iSp - 1] = tan(save[iSp - 1]);
			break;

		case POLY_CSC:
			if (!(t = sin(save[iSp - 1])))
				return 0;

			save[iSp - 1] = 1 / t;
			break;

		case POLY_SEC:
			if (!(t = cos(save[iSp - 1])))
				return 0;

			save[iSp - 1] = 1 / t;
			break;

		case POLY_COT:
			if (!(t = sin(save[iSp - 1])))
				return 0;

			save[iSp - 1] = cos(save[iSp - 1]) / t;
			break;

		case POLY_LN:
			if (save[iSp - 1] <= 0)
				return 0;

			save[iSp - 1] = log(save[iSp - 1]);
			break;

		case POLY_LOG10:
			if (save[iSp - 1] <= 0)
				return 0;

			save[iSp - 1] = log10(save[iSp - 1]);
			break;

		case POLY_LOG:
			if (save[iSp - 1] <= 0)
				return 0;

			if (save[iSp - 2] <= 0 || save[iSp - 2] == 1)
				return 0;

			save[iSp - 2] = log(save[iSp - 1]) / log(save[iSp - 2]);
			iSp--;
			break;

		case POLY_ABS:
			save[iSp - 1] = fabs(save[iSp - 1]);
			break;

		case POLY_FLOOR:
			save[iSp - 1] = floor(save[iSp - 1]);
			break;

		case POLY_IRAND:
			save[iSp - 2] = my_irandom(save[iSp - 2], save[iSp - 1]);
			iSp--;
			break;

		case POLY_FRAND:
			save[iSp - 2] = my_frandom(save[iSp - 2], save[iSp - 1]);
			iSp--;
			break;

		case POLY_MINF:
			save[iSp - 2] = (save[iSp - 2] < save[iSp - 1]) ? save[iSp - 2] : save[iSp - 1];
			iSp--;
			break;

		case POLY_MAXF:
			save[iSp - 2] = (save[iSp - 2] > save[iSp - 1]) ? save[iSp - 2] : save[iSp - 1];
			iSp--;
			break;

		default:
			return 0;
		}
	}
	return save[iSp - 1];
}

int CPoly::Analyze(const char* pszStr)
{
	if (pszStr)
		SetStr(pszStr);

	if (0 == strData.length())
		return true;

	ErrorOccur = false;
	uiLookPos = 0;
	iLookAhead = lexan();

	expr();

	if (tokenBase.empty())
		return false;

	return !ErrorOccur;
}

void CPoly::Clear()
{
	int i;
	tokenBase.clear();
	numBase.clear();

	for (i = 0; i < STSize; ++i) {
		if (lSymbol[i]) {
			delete lSymbol[i];
			lSymbol[i] = NULL;
		}
	}

	lSymbol.clear();
	SymbolIndex.clear();
	STSize = 0;
}

void CPoly::expr()
{
	int t;

	switch (iLookAhead) {
	case '+':
	case '-':
		uiLookPos--;
		iLookAhead = POLY_NUM;
		iNumToken = iToken = 0;
	}

	term();

	while (!ErrorOccur) {
		switch (iLookAhead) {
		case '+':
		case '-':
			t = iLookAhead;
			match(t);
			term();
			emit(t, POLY_NONE);
			continue;

		case POLY_EOS:
		case ')':
		case ',':
			return;

		default:
			error();
			return;
		}
	}
}

void CPoly::error()
{
	iErrorPos = uiLookPos;
	ErrorOccur = true;
}

int CPoly::lexan()
{
	int t;
	double tt;

	while (uiLookPos < strData.size()) {
		if (strData[uiLookPos] == ' ' || strData[uiLookPos] == '\t')
			;
		else if (isdigit(strData[uiLookPos])) {
			t = 0;
			for (; uiLookPos < strData.size(); uiLookPos++) {
				if (isdigit(strData[uiLookPos]))
					t = t * 10 + strData[uiLookPos] - '0';
				else
					break;
			}

			iToken = t;
			tt = 0.1;
			iNumToken = 0;

			if (uiLookPos < strData.size() && strData[uiLookPos] == '.')
			{
				uiLookPos++;
				for (; uiLookPos < strData.size(); uiLookPos++, tt *= 0.1) {
					if (isdigit(strData[uiLookPos]))
						iNumToken += tt * (strData[uiLookPos] - '0');
					else
						break;
				}
			}

			iNumToken += iToken;
			return POLY_NUM;
		}
		else if (isalpha(strData[uiLookPos])) {
			std::string localSymbol("");
			while (uiLookPos < strData.size() && isalpha(strData[uiLookPos])) {
				localSymbol += strData[uiLookPos];
				uiLookPos++;
			}

			iToken = find(localSymbol);
			if (iToken == -1)
				iToken = insert(localSymbol, POLY_ID);

			return lSymbol[(/*FindIndex*/(iToken))]->token;
		}
		else {
			iToken = 0;
			return strData[uiLookPos++];
		}

		uiLookPos++;
	}

	return POLY_EOS;
}

void CPoly::term()
{
	int t;
	factor();

	while (!ErrorOccur) {
		switch (iLookAhead) {
		case '*':
		case '/':
		case '%':
			t = iLookAhead;
			match(t);
			factor();
			emit(t, POLY_NONE);
			continue;

		default:
			return;
		}
	}
}

void CPoly::factor()
{
	int t;
	expo();

	while (!ErrorOccur) {
		switch (iLookAhead) {
		case '^':
			t = iLookAhead;
			match(t);
			expo();
			emit(t, POLY_NONE);
			continue;

		default:
			return;
		}
	}
}

void CPoly::expo()
{
	int t;
	switch (iLookAhead) {
	case '(':
		match('(');
		expr();
		match(')');
		break;

	case POLY_NUM:
		emit(POLY_NUM, iToken);
		match(POLY_NUM);
		break;

	case POLY_ID:
		emit(POLY_ID, iToken);
		match(POLY_ID);
		break;

	case POLY_ROOT:
	case POLY_SIN:
	case POLY_COT:
	case POLY_TAN:
	case POLY_CSC:
	case POLY_SEC:
	case POLY_LN:
	case POLY_LOG10:
	case POLY_COS:
	case POLY_ABS:
	case POLY_FLOOR:
	case POLY_SIGN:
		t = iLookAhead;
		match(iLookAhead);
		match('(');
		expr();
		match(')');
		emit(t, iToken);
		break;

	case POLY_LOG:
	case POLY_MINF:
	case POLY_MAXF:
	case POLY_IRAND:
	case POLY_FRAND:
	case POLY_MOD:
		t = iLookAhead;
		match(iLookAhead);
		match('(');
		expr();
		match(',');
		expr();
		match(')');
		emit(t, iToken);
		break;

	case POLY_EOS:
		break;

	default:
		error();
	}
}

void CPoly::match(int t)
{
	if (iLookAhead == t)
		iLookAhead = lexan();
	else
		error();
}

void CPoly::emit(int t, int tval)
{
	switch (t) {
	case '+':
		tokenBase.push_back(POLY_PLU);
		break;

	case '-':
		tokenBase.push_back(POLY_MIN);
		break;

	case '*':
		tokenBase.push_back(POLY_MUL);
		break;

	case '/':
		tokenBase.push_back(POLY_DIV);
		break;

	case '%':
		tokenBase.push_back(POLY_MOD);
		break;

	case '^':
		tokenBase.push_back(POLY_POW);
		break;

	case POLY_ROOT:
	case POLY_SIN:
	case POLY_TAN:
	case POLY_COT:
	case POLY_COS:
	case POLY_CSC:
	case POLY_SEC:
	case POLY_LOG:
	case POLY_LN:
	case POLY_LOG10:
	case POLY_ABS:
	case POLY_MINF:
	case POLY_MAXF:
	case POLY_IRAND:
	case POLY_FRAND:
	case POLY_MOD:
	case POLY_FLOOR:
	case POLY_SIGN:
		tokenBase.push_back(t);
		break;

	case POLY_NUM:
		tokenBase.push_back(t);
		numBase.push_back(iNumToken);
		break;

	case POLY_ID:
		tokenBase.push_back(t);
		tokenBase.push_back(tval);
		break;

	default:
		error();
		Clear();
		return;
	}
}

int CPoly::find(const std::string & s)
{
	int l, m, r;

	l = 0;
	r = STSize - 1;

	while (l <= r) {
		m = (l + r) >> 1;

		if (lSymbol[SymbolIndex[m]]->strlex == s)
			return SymbolIndex[m];
		else if (lSymbol[SymbolIndex[m]]->strlex < s)
			l = m + 1;
		else
			r = m - 1;
	}

	return -1;
}

int CPoly::insert(const std::string & s, int tok)
{
	int i;
	bool bAdded = false;

	lSymbol.push_back(new CSymTable(tok, s));

	for (i = 0; i < STSize; i++) {
		if (s < lSymbol[SymbolIndex[i]]->strlex) {
			SymbolIndex.insert(SymbolIndex.begin() + i, STSize);
			bAdded = true;
			break;
		}
	}

	if (!bAdded)
		SymbolIndex.push_back(STSize);

	STSize++;
	return STSize - 1;
}

int CPoly::SetVar(const std::string & strName, double dVar)
{
	if (ErrorOccur)
		return false;

	int index = find(strName);
	if (index == -1)
		return false;

	CSymTable * stVar = lSymbol[index];
	stVar->dVal = dVar;
	return true;
}

double CPoly::GetVar(const std::string & strName)
{
	if (ErrorOccur)
		return false;

	int index = find(strName);
	if (index == -1)
		return false;

	CSymTable * stVar = lSymbol[index];
	if (!stVar)
		return -1;

	return stVar->dVal;
}

void CPoly::init()
{
	insert("min", POLY_MINF);
	insert("max", POLY_MAXF);
	insert("number", POLY_IRAND);
	insert("irandom", POLY_IRAND);
	insert("irand", POLY_IRAND);
	insert("frandom", POLY_FRAND);
	insert("frand", POLY_FRAND);
	insert("rt", POLY_ROOT);
	insert("sqrt", POLY_ROOT);
	insert("cos", POLY_COS);
	insert("sin", POLY_SIN);
	insert("tan", POLY_TAN);
	insert("cot", POLY_COT);
	insert("csc", POLY_CSC);
	insert("cosec", POLY_COSEC);
	insert("sec", POLY_SEC);
	insert("pi", POLY_PI);
	SetVar("pi", 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068);
	insert("e", POLY_EXP);
	SetVar("e", 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427);
	insert("log", POLY_LOG);
	insert("ln", POLY_LN);
	insert("log10", POLY_LOG10);
	insert("abs", POLY_ABS);
	insert("mod", POLY_MOD);
	insert("floor", POLY_FLOOR);
	insert("sign", POLY_SIGN);
}

int CPoly::my_irandom(double start, double end)
{
	// Make range as inclusive-exclusive
	int is = int(start + 0.5);
	int ie = int(end - start + 0.5) + 1;

	return int(_random() * ie + is);
}

double CPoly::my_frandom(double start, double end)
{
	return _random()* (end - start) + start;
}