/**
 * @file	Md5.java
 * @author	Sebastien GUILLERM
 * @brief	MD5 functions.
 */

package com.mobii.utils;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Md5 {

	public Md5() {}
	public Md5(Md5 m) {}

	/**
	 * @brief	String to MD5 conversion.
	 * @details	Encodes a string to MD5.
	 * @param	str	The string to encode.
	 * @return	A string which is the result of the MD5 conversion of <b>str</b>.
	 */
	public String encode(String str) {
		byte[] uniqueKey = str.getBytes();
        byte[] hash = null;

        try {
        	hash = MessageDigest.getInstance("MD5").digest(uniqueKey);
        } catch (NoSuchAlgorithmException e) {
        	throw new Error("No MD5 support in this VM.");
        }

        StringBuilder hashString = new StringBuilder();
        for (int i = 0; i < hash.length; i++) {
        	String hex = Integer.toHexString(hash[i]);
        	if (hex.length() == 1) {
        		hashString.append('0');
        		hashString.append(hex.charAt(hex.length() - 1));
        	} else
        		hashString.append(hex.substring(hex.length() - 2));
        }
        return hashString.toString();
	}
}
