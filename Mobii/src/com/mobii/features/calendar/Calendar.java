package com.mobii.features.calendar;

public class Calendar {

	public Calendar() {}
	public Calendar(Calendar c) {}

	public String	getEvents() {
		return new GetEvents().getEvents();
	}

	public void		addEvent(String event) {
		new AddEvent().addEvent(event);
	}

	public void		delEvent(String event) {
		new DelEvent().delEvent(event);
	}

	public void		uptEvent(String event) {
		new UptEvent().uptEvent(event);
    }
}
