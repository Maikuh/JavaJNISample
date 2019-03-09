// SalonsJNI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "json.hpp"
#include "com_markup_salon_models_JNI.h"

using json = nlohmann::json;
using namespace std;

string jstringToString(JNIEnv *env, jstring jStr) {
	if (!jStr)
		return "";

	const jclass stringClass = env->GetObjectClass(jStr);
	const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
	const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

	size_t length = (size_t)env->GetArrayLength(stringJbytes);
	jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

	std::string ret = std::string((char *)pBytes, length);
	env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

	env->DeleteLocalRef(stringJbytes);
	env->DeleteLocalRef(stringClass);
	return ret;
}

JNIEXPORT void JNICALL Java_com_markup_salon_models_JNI_salonsDataToTxt
(JNIEnv *env, jobject obj, jstring jsonString) {
	json j = json::parse(jstringToString(env, jsonString));
	ofstream file("salonsData.json");
	file << setw(4) << j << endl;
	file.close();
}
