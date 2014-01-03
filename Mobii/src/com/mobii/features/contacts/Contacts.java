/**
 * @file	Contacts.java
 * @author	Sebastien GUILLERM
 * @brief	Manages the list of contacts.
 */

package com.mobii.features.contacts;

public class Contacts {

	public Contacts() {}
	public Contacts(Contacts c) {}

	public String	getContacts() {
		return new GetContacts().getContacts();
	}

	public void		addContact(String contact) {
		new AddContact().addContact(contact);
	}

	public void		delContact(String contact) {
		new DelContact().delContact(contact);
	}

	public void		uptContact(String contact) {
		new UptContact().uptContact(contact);
    }
}
