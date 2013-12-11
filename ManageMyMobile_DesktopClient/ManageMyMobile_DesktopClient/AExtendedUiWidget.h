#ifndef AEXTENDEDUIWIDGET_H_
#define AEXTENDEDUIWIDGET_H_

/*! \file */

/*! \class AExtendedUiWidget
**	\brief Adds additional events to Qt UI components.
**	Provides events handlers for Mobii panels, especially when a panel is opened and another one is hidden.
*/
class				AExtendedUiWidget
{
public:
	virtual ~AExtendedUiWidget(void) {}

	///	Panel is brought to foreground.
	/// This event is triggered when the panel which implements it is brought to front.
	virtual void	OnWidgetShow(void);

	///	Panel is going to be hidden.
	/// This event is triggered when another panel is going to be brought to foreground.
	virtual void	OnWidgetClose(void);
};


#endif // !AEXTENDEDUIWIDGET_H_