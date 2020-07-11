class CAutoQueryManager : public singleton<CAutoQueryManager>
{
public:
	bool		Initialize();
	void		Destroy();	
	void		UpdateQuestFlags();
	void		UpdateItems();
	void		OldProcess();
};