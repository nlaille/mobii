#ifndef INTERCLIENTPROTOCOLHELPER_H_
#define INTERCLIENTPROTOCOLHELPER_H_

#include "qvariant.h"

/*! \file */

/**
 \brief Provides basic functionalities to read from and write to JSON.
 JSON is the designated protocol for communications between desktop clients and phone apps.
 
 Uses a third party JSON library, courtesy of Eeli Reilin.
 https://github.com/da4c30ff/qt-json
 
 Below is an example of use of the *inter-client protocol* specifications.
 
 CAUTION: this protocol is not natively designed to handle binary datas (e.g. raw files or pictures).
 A workaround using Base64 may be used for that purpose.
 
 \code{.cpp}
 {
 	"action" : "mobile_infos"
 	"args" : {
 		"manufacturer" : "Samsung"
 		"model" : "GT-I9300"
 		"imei" : "359336049398873"
 		[...]
 	}
 }
 \endcode
 */
namespace IcpHelper
{
	/// Converts a key-value map to a plain string JSON.
	QVariantMap			jsonToData(QString &inputJson);

	/// Converts a plain string JSON to a key-value map.
	QByteArray			dataToJson(QVariant inputDatas);
}

#endif // !INTERCLIENTPROTOCOLHELPER_H_