/**
 * @file	Files.java
 * @author	Sebastien GUILLERM
 * @brief	Functions concerning files.
 */

package com.mobii.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.util.Log;

public class Files {

	public Files() {}
	public Files(Files f) {}

	/**
	 * @brief	Reads a file.
	 * @details	Reads and gets the content of a file.
	 * @param	file	The file to be read.
	 * @return	A string which is the content of the file.
	 */
	public String	readFile(File file) {
		BufferedReader bufRead = null;
		StringBuilder builder = new StringBuilder();
		String line;

		try {
			bufRead = new BufferedReader(new FileReader(file));
			while ((line = bufRead.readLine()) != null)
				builder.append(line);
		} catch (Exception e) {
			Log.e("Read file", e.toString());
		} finally {
			if (bufRead != null) {
				try {
					bufRead.close();
				} catch (IOException e) {
					Log.e("Close file", e.toString());
				}
			}
		}
		return builder.toString();
	}
}
