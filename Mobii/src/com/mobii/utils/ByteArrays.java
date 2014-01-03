/**
 * @file	ByteArrays.java
 * @author	Sebastien GUILLERM
 * @brief	Functions concerning byte arrays.
 */

package com.mobii.utils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ByteArrays {

	public ByteArrays() {}
	public ByteArrays(ByteArrays ba) {}

	/**
	 * @brief	Integer to byte array conversion.
	 * @details	Converts an integer to a byte array.
	 * @param	val	The integer to convert.
	 * @return	A byte array which is the conversion of <b>val</b>.
	 */
	public byte[]	intToByteArray(int val) {
	    return new byte[] {
	    		(byte)val,
	            (byte)(val << 8),
	            (byte)(val << 16),
	            (byte)(val << 24)};
	}

	/**
	 * @brief	Integer to byte array conversion.
	 * @details	Converts the integer value of a string to a byte array.
	 * @param	val	The string to convert.
	 * @return	A byte array which is the conversion of the integer value of <b>val</b>.
	 */
	public byte[]	intToByteArray(String val) {
		int value = Integer.valueOf(val);
	    return new byte[] {
	    		(byte)value,
	            (byte)(value << 8),
	            (byte)(value << 16),
	    		(byte)(value << 24)};
	}

	/**
	 * @brief	Integer to byte array conversion.
	 * @details	Converts an integer to a byte array in little endian.
	 * @param	val	The integer to convert.
	 * @return	A byte array which is the conversion of <b>val</b>.
	 */
	public byte[]	intToByteArray_le(int val) {
		return ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(val).array();
	}

	/**
	 * @brief	Integer to byte array conversion.
	 * @details	Converts an integer to a byte array in big endian.
	 * @param	val	The integer to convert.
	 * @return	A byte array which is the conversion of <b>val</b>.
	 */
	public byte[]	intToByteArray_be(int val) {
		return ByteBuffer.allocate(4).order(ByteOrder.BIG_ENDIAN).putInt(val).array();
	}

	/**
	 * @brief	Byte array to integer conversion.
	 * @details	Converts a byte array to an integer in little endian.
	 * @param	val	The byte array to convert.
	 * @return	An integer which is the conversion of <b>val</b>.
	 */
	public int		byteArrayToint_le(byte[] val) {
		return ByteBuffer.wrap(val).order(ByteOrder.LITTLE_ENDIAN).getInt();
	}

	/**
	 * @brief	Byte array to integer conversion.
	 * @details	Converts a byte array to an integer in big endian.
	 * @param	val	The byte array to convert.
	 * @return	An integer which is the conversion of <b>val</b>.
	 */
	public int		byteArrayToint_be(byte[] val) {
		return ByteBuffer.wrap(val).order(ByteOrder.BIG_ENDIAN).getInt();
	}

	/**
	 * @brief	Byte array to hex string conversion.
	 * @details	Converts a byte array to a hexadecimal string.
	 * @param	array	The byte array to convert.
	 * @return	A string which is the conversion of <b>array</b>.
	 */
	public String	byteArrayToHex(byte[] array) {
		StringBuilder sb = new StringBuilder();
		for(byte b: array)
			sb.append(String.format("%02x", b&0xff));
		return sb.toString();
	}

	/**
	 * @brief	Concatenates byte arrays.
	 * @details	Concatenates all the byte arrays passed in arguments.
	 * @param	arrays	The list of byte arrays.
	 * @return	A byte array which is the concatenation of <b>arrays</b>.
	 */
	public byte[]	concatByteArrays(byte[]...arrays) {
		int len = 0;
	    for (int i = 0; i < arrays.length; i++) {
	        len += arrays[i].length;
	    }
	    byte[] concatArray = new byte[len];
	    int pos = 0;
	    for (int i = 0; i < arrays.length; i++) {
	        System.arraycopy(arrays[i], 0, concatArray, pos, arrays[i].length);
	        pos += arrays[i].length;
	    }
	    return concatArray;
	}
}
