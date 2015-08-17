#ifndef ELEMENTAL_H
#define ELEMENTAL_H

class Elemental
{
	public:
		enum ELEMENT_TYPE
		{
			NORMAL_TYPE,
			LIGHTNING_TYPE,
			WATER_TYPE,
			EARTH_TYPE,
			NUM_ELEMENTS
		};

	protected:
		ELEMENT_TYPE m_element;

	public:
		Elemental();
		virtual ~Elemental();

		ELEMENT_TYPE GetElement(void);
		bool IsNeutralTo(ELEMENT_TYPE element);
		bool IsVulnerableTo(ELEMENT_TYPE element);
		bool IsInvulnerableTo(ELEMENT_TYPE element);
};

#endif