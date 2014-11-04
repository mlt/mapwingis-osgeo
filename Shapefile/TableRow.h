// TableRow.h: interface for the TableRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEROW_H__082441FB_7C83_40DE_B1EE_D560225A5D2F__INCLUDED_)
#define AFX_TABLEROW_H__082441FB_7C83_40DE_B1EE_D560225A5D2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TableRow  
{
public:
    enum DataStatusType { DATA_CLEAN, DATA_INSERTED, DATA_MODIFIED };

	TableRow();
	virtual ~TableRow();

    DataStatusType status()
    {
        return _status;
    }
    void SetDirty(DataStatusType nStatus)
    {
        _status = nStatus;
    }
	TableRow* Clone()
	{
		TableRow* row = new TableRow();
		for (int i = 0; i < (int)values.size(); i++)
		{
			VARIANT* varNew = new VARIANT();
			if (values[i] != NULL) {
				VariantCopy(varNew, values[i]);
			}
			else {
				VariantInit(varNew);
			}
			row->values.push_back(varNew);
		}
		return row;
	}
	std::vector<VARIANT *> values;
	VARIANT* GetValue(int index) {
		return (index >= 0 && index < (int)values.size()) ? values[index] : NULL;
	}
private:
    DataStatusType _status;
};

#endif // !defined(AFX_TABLEROW_H__082441FB_7C83_40DE_B1EE_D560225A5D2F__INCLUDED_)
