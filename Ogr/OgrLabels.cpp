#include "stdafx.h"
#include "OgrLabels.h"
#include "OgrHelper.h"

#define LABEL_X_FIELD "labelX"
#define LABEL_Y_FIELD "labelY"
#define LABEL_TEXT_FIELD "labelText"
#define LABEL_ROTATION_FIELD "labelRotation"

// *************************************************************
//		GetLabelFields()
// *************************************************************
bool OgrLabelsHelper::GetLabelFields(OGRLayer* ogrLayer, LabelFields& fields)
{
	OGRFeatureDefn* fd = ogrLayer->GetLayerDefn();
	fields.X = fd->GetFieldIndex(LABEL_X_FIELD);
	fields.Y = fd->GetFieldIndex(LABEL_Y_FIELD);
	fields.Text = fd->GetFieldIndex(LABEL_TEXT_FIELD);
	fields.Rotation = fd->GetFieldIndex(LABEL_ROTATION_FIELD);
	return fields.HasFields();
}

// *************************************************************
//		AddFieldsForLabels()
// *************************************************************
bool OgrLabelsHelper::AddFieldsForLabels(OGRLayer* ogrLayer)
{
	CComBSTR fields[4];
	fields[0].Attach(A2BSTR(LABEL_X_FIELD));
	fields[1].Attach(A2BSTR(LABEL_Y_FIELD));
	fields[2].Attach(A2BSTR(LABEL_TEXT_FIELD));
	fields[3].Attach(A2BSTR(LABEL_ROTATION_FIELD));

	for (int i = 0; i < 4; i++)
	{
		OGRFieldDefn oField(OgrHelper::Bstr2OgrString(fields[i].m_str), i == 2 ? OFTString : OFTReal);
		oField.SetWidth(i == 2 ? 128 : 0);
		oField.SetPrecision(8);

		if (ogrLayer->CreateField(&oField) != OGRERR_NONE)
			return false;
	}
	return true;
}

// *************************************************************
//		AddLabelFeature()
// *************************************************************
bool OgrLabelsHelper::AddLabel2Feature(ILabels* labels, int shapeIndex, OGRFeature* feature, LabelFields& labelFields)
{
	if (!labels || !feature) return false;

	CComPtr<ILabel> label = NULL;
	labels->get_Label(shapeIndex, 0, &label);

	CComBSTR text;
	double x, y, rotation;
	label->get_X(&x);
	label->get_Y(&y);
	label->get_Rotation(&rotation);
	label->get_Text(&text);

	feature->SetField(labelFields.X, x);
	feature->SetField(labelFields.Y, y);
	feature->SetField(labelFields.Rotation, rotation);
	feature->SetField(labelFields.Text, OgrHelper::Bstr2OgrString(text.m_str));
	return false;
}
