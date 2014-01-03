/**
 * @file	IProtocolAction.java
 * @author	Sebastien GUILLERM
 * @brief	Action protocol interface.
 */

package com.mobii.protocol;

public interface IProtocolAction {

	public enum cliToMob {
		REQ_SMS, REQ_SENDSMS,
		REQ_CONTACTS, UPT_CONTACTS, NEW_CONTACT, DEL_CONTACT,
		REQ_CALENDAR, UPT_EVENTS, NEW_EVENT, DEL_EVENT,
		REQ_IMAGES, DEL_IMAGE, REQ_THIS_IMAGE;
	}

	public enum mobToCli {
		MOBILE_INFOS,
		SMS,
		CONTACTS, VALID_NEW_CONTACT,
		CALENDAR, VALID_NEW_EVENT,
		FILE, IMAGELIST;
	}
}
