// RFLayer.h: interface for the RFLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFLAYER_H__7E9EE14D_AB39_4D89_8E1F_105867CA055B__INCLUDED_)
#define AFX_RFLAYER_H__7E9EE14D_AB39_4D89_8E1F_105867CA055B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RFLayer  
{
public:
	RFLayer();
	virtual ~RFLayer();
	RFLayer(const RFLayer&);	// copy constructor
	RFLayer& operator=(const RFLayer&);	// assignment operator
	
	 // Water Resistance Factors Parameters
	 double RFwMin,
			  RFwMax,
			  nWat,
	 
	 // Polymer RFOil Parameters
		     RFoMin,
	        RFoMax,
	        nOil;

private:
	void InitRFLayer();
	void Equate(const RFLayer&);
};

#endif // !defined(AFX_RFLAYER_H__7E9EE14D_AB39_4D89_8E1F_105867CA055B__INCLUDED_)
