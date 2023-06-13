#if !defined( __LAYER_PROPERTIES_H )
#define __LAYER_PROPERTIES_H

#include "StdAfx.h"
#include "StaticLayerData.h"
#include "DynamicLayerData.h"


class LayerProperties
{
	
public:
	LayerProperties();				// constructor
	LayerProperties(int nWater, int nOil);				// constructor
	~LayerProperties();				// destructor
	LayerProperties(const LayerProperties&);	// copy constructor
	LayerProperties& operator=(const LayerProperties&);	// assignment operator

	void		Add(const double&);	// add a device to the collection
	void		Remove(int);			// remove a device by index
	void		RemoveAll();			// remove all devices
	void		Sort();					// currently empty unused function


	StaticLayerData staticLayer;
	DynamicLayerData dynamicLayer;

private:
	void Allocate();
	void Deallocate();
	void InitLayerProperties();
	void Equate(const LayerProperties&);	// function common to copy const. & op=


};


#endif