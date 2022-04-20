#include <jni.h>
#include <string>
#include <sodium.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>


#include "native-lib.hpp"
#include "test_decaf.hpp"
#include "tot-helper.hpp"
#include "flatbuffers/flatbuffers.h"
#include "libtot/libtot_client_native.hpp"
#include "libtot/Message.hpp"
#include "libtot/libtot_network.hpp"

using namespace libtot;

std::string hello_world_helper() {
    std::string hello = "Hello from C++ helper";

    if (!test_decaf()) {
        return "FATAL ERROR";
    } else {
        hello = "Hello from decaf execution";
    }

    if (sodium_init() < 0) {
        return "FATAL ERROR";
    } else {
        hello = "Hello from decaf and sodium";
    }

    return hello;
}

void loadJavaArray(JNIEnv *env, const jbyteArray &arr, jbyte *&ptr, jsize &len) {
    ptr = env->GetByteArrayElements(arr, nullptr);
    len = env->GetArrayLength(arr);
}

extern "C" JNIEXPORT jstring JNICALL
Java_me_zhanghan177_tot_1mobile_1native_activities_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = hello_world_helper();
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_activities_MainActivity_integerFromJNI(JNIEnv *env,
                                                                               jobject thiz) {
    return MAGIC_NUMBER;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_me_zhanghan177_tot_1mobile_1native_activities_MainActivity_integerObjectFromJNI(JNIEnv *env,
                                                                                     jobject thiz) {
    jclass cls = env->FindClass("java/lang/Integer");
    jmethodID midInit = env->GetMethodID(cls, "<init>", "(I)V");
    if (nullptr == midInit) return nullptr;
    jobject newObj = env->NewObject(cls, midInit, MAGIC_NUMBER);

    return newObj;
}

extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_generateKeypairJNI(JNIEnv *env,
                                                                              jobject thiz) {
    std::vector<int8_t> pubkey;
    std::vector<int8_t> privkey;
    libtot::generate_keypair(pubkey, privkey);

    jclass clazz = env->FindClass("me/zhanghan177/tot_mobile_native/TOTKeyStoreService");

    // Update Java's object for user key pair
    // Update public key
    jbyteArray pubkey_array = env->NewByteArray(pubkey.size());
    env->SetByteArrayRegion(pubkey_array, 0, pubkey.size(), &pubkey[0]);

    jmethodID midSetPubkey = env->GetMethodID(clazz, "setUserPubkey", "([B)V");
    env->CallVoidMethod(thiz, midSetPubkey, pubkey_array);

    // Update private key
    jbyteArray privkey_array = env->NewByteArray(privkey.size());
    env->SetByteArrayRegion(privkey_array, 0, privkey.size(), &privkey[0]);

    jmethodID midSetPrivkey = env->GetMethodID(clazz, "setUserPrivkey", "([B)V");
    env->CallVoidMethod(thiz, midSetPrivkey, privkey_array);

    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_getAsymmetricEncryptionKeySet_1FULL_1PK_1SIZE_1JNI(
        JNIEnv *env, jobject thiz) {
    return libtot::AsymmetricEncryptionKeySet::FULL_PK_SIZE;
}extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_getG_1CHALLENGE_1SIZE_1JNI(JNIEnv *env,
                                                                                      jobject thiz) {
    return libtot::G_CHALLENGE_SIZE;
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_random_1buf_1JNI(JNIEnv *env,
                                                                            jobject thiz) {
    jbyte buf[libtot::G_CHALLENGE_SIZE]{};
    libtot::random_buf(buf, sizeof(buf));
    jbyteArray res = env->NewByteArray(sizeof(buf));
    env->SetByteArrayRegion(res, 0, sizeof(buf), buf);
    return res;
}

extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_prepareInitializationRequestJNI(
        JNIEnv *env, jobject thiz, jbyteArray user_pubkey, jbyteArray user_privkey,
        jbyteArray device_secret, jstring device_ip_in, jint device_port_in,
        jbyteArray device_pubkey) {
    jbyte *user_pubkey_ptr;
    jsize user_pubkey_len;
    loadJavaArray(env, user_pubkey, user_pubkey_ptr, user_pubkey_len);

    jbyte *user_privkey_ptr;
    jsize user_privkey_len;
    loadJavaArray(env, user_privkey, user_privkey_ptr, user_privkey_len);

    jbyte *device_secret_ptr;
    jsize device_secret_len;
    loadJavaArray(env, device_secret, device_secret_ptr, device_secret_len);

    const char *device_ip_load = env->GetStringUTFChars(device_ip_in, nullptr);

    jbyte *device_pubkey_ptr;
    jsize device_pubkey_len;
    loadJavaArray(env, device_pubkey, device_pubkey_ptr, device_pubkey_len);

    SharedSecretKey setup_token = SharedSecretKey(
            reinterpret_cast<const uint8_t *>(device_secret_ptr), device_secret_len);

    AsymmetricEncryptionKeySet keySet = AsymmetricEncryptionKeySet(
            reinterpret_cast<const uint8_t *>(user_pubkey_ptr), user_pubkey_len,
            reinterpret_cast<const uint8_t *>(user_privkey_ptr), user_privkey_len);

    return admin_initialize_device_impl(device_ip_load, device_port_in, user_pubkey_ptr,
                                        user_pubkey_len,
                                        setup_token, keySet);

}

AsymmetricEncryptionKeySet getUserKeySet(JNIEnv *env,
                                         jbyteArray user_pubkey,
                                         jbyteArray user_privkey) {
    jbyte *user_pubkey_ptr;
    jsize user_pubkey_len;
    loadJavaArray(env, user_pubkey, user_pubkey_ptr, user_pubkey_len);

    jbyte *user_privkey_ptr;
    jsize user_privkey_len;
    loadJavaArray(env, user_privkey, user_privkey_ptr, user_privkey_len);

    return AsymmetricEncryptionKeySet(
            reinterpret_cast<const uint8_t *>(user_pubkey_ptr), user_pubkey_len,
            reinterpret_cast<const uint8_t *>(user_privkey_ptr), user_privkey_len);

}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_acquirePreAuthTokenJNI(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jbyteArray user_pubkey,
                                                                                  jbyteArray user_privkey,
                                                                                  jstring admin_ip_in,
                                                                                  jint admin_port_in,
                                                                                  jbyteArray admin_pubkey) {

    const char *admin_ip_load = env->GetStringUTFChars(admin_ip_in, nullptr);

    jbyte *admin_pubkey_ptr;
    jsize admin_pubkey_len;
    loadJavaArray(env, admin_pubkey, admin_pubkey_ptr, admin_pubkey_len);

    PreAuthToken token;

    AsymmetricEncryptionKeySet keySet = getUserKeySet(env, user_pubkey, user_privkey);

    int err = user_acquire_pre_auth_token_impl(admin_ip_load, admin_port_in,
                                               reinterpret_cast<const uint8_t *>(admin_pubkey_ptr),
                                               keySet, token);

    jbyteArray ret = env->NewByteArray(libtot::PreAuthToken::get_token_len());

    if (err != 0) {
        jbyte emp[libtot::PreAuthToken::PRE_AUTH_TOKEN_SIZE]{0};
        env->SetByteArrayRegion(ret, 0, libtot::PreAuthToken::get_token_len(), emp);
    } else {
        env->SetByteArrayRegion(ret, 0, libtot::PreAuthToken::get_token_len(),
                                reinterpret_cast<const jbyte *>(token.get_token()));
    }
    return ret;
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_claimDeviceJNI(JNIEnv *env, jobject thiz,
                                                                          jbyteArray user_pubkey,
                                                                          jbyteArray user_privkey,
                                                                          jbyteArray pre_auth_token,
                                                                          jstring device_ip_in,
                                                                          jint device_port_in,
                                                                          jbyteArray admin_pubkey) {
    AsymmetricEncryptionKeySet keySet = getUserKeySet(env, user_pubkey, user_privkey);

    jbyte *pre_auth_token_ptr;
    jsize pre_auth_token_len;
    loadJavaArray(env, pre_auth_token, pre_auth_token_ptr, pre_auth_token_len);
    PreAuthToken token(reinterpret_cast<const uint8_t *>(pre_auth_token_ptr), pre_auth_token_len);

    const char *device_ip_load = env->GetStringUTFChars(device_ip_in, nullptr);

    jbyte *admin_pubkey_ptr;
    jsize admin_pubkey_len;
    loadJavaArray(env, admin_pubkey, admin_pubkey_ptr, admin_pubkey_len);

    uint8_t claimed_device[AsymmetricEncryptionKeySet::FULL_PK_SIZE];

    int err = user_claim_device_impl(keySet, token,
                                     device_ip_load, device_port_in,
                                     reinterpret_cast<const uint8_t *>(admin_pubkey_ptr),
                                     false, claimed_device, sizeof(claimed_device));

    if (err != 0) {
        // Claim failed.
        memset(claimed_device, 0, sizeof(claimed_device));
    }

    jbyteArray ret = env->NewByteArray(sizeof(claimed_device));
    env->SetByteArrayRegion(ret, 0, sizeof(claimed_device),
                            reinterpret_cast<const jbyte *>(claimed_device));
    return ret;
}extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_registerIPKmsJNI(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jbyteArray client_pubkey,
                                                                            jstring client_ip_in,
                                                                            jint client_port_in,
                                                                            jstring storage_ip_in,
                                                                            jint storage_port_in) {
    jbyte *client_pubkey_ptr;
    jsize client_pubkey_len;
    loadJavaArray(env, client_pubkey, client_pubkey_ptr, client_pubkey_len);

    const char *client_ip_load = env->GetStringUTFChars(client_ip_in, nullptr);

    const char *storage_ip_load = env->GetStringUTFChars(storage_ip_in, nullptr);

    return client_register_ip_kms_impl(reinterpret_cast<const uint8_t *>(client_pubkey_ptr),
                                       client_pubkey_len,
                                       client_ip_load, client_port_in,
                                       storage_ip_load, storage_port_in);
}extern "C"
JNIEXPORT jint JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_getMessageTypeFltbuffersSizeJNI(
        JNIEnv *env, jobject thiz) {
    return libtot::get_message_type_flatbuffers_size();
}extern "C"
JNIEXPORT jstring JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_resolveIpJNI(JNIEnv *env, jobject thiz,
                                                                        jbyteArray query_pubkey,
                                                                        jstring storage_ip_in,
                                                                        jint storage_port_in) {
    jbyte *query_pubkey_ptr;
    jsize query_pubkey_len;
    loadJavaArray(env, query_pubkey, query_pubkey_ptr, query_pubkey_len);

    const char *storage_ip_load = env->GetStringUTFChars(storage_ip_in, nullptr);

    std::string res_ip;
    int res_port;

    client_fetch_ip_kms_impl(reinterpret_cast<const uint8_t *>(query_pubkey_ptr), query_pubkey_len,
                             storage_ip_load, storage_port_in,
                             res_ip, res_port);

    jstring result;
    result = env->NewStringUTF(res_ip.c_str());
    return result;
}

bool checkMessageType(JNIEnv *env, jbyteArray message_type, MessageType exp_type) {
    jbyte *message_type_ptr;
    jsize message_type_len;
    loadJavaArray(env, message_type, message_type_ptr, message_type_len);

    int message_size = libtot::get_message_type_flatbuffers_size();
    assert(message_size != 0 && message_type_len >= message_size);

    auto type_msg = GetMessageTypeMsg(message_type_ptr);
    return (type_msg != nullptr && type_msg->type() == exp_type);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTAdminService_checkMessageTypeAdminPreAuthJNI(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jbyteArray message_type) {
    return checkMessageType(env, message_type, MessageType_ACQUIRE_PRE_AUTH_TOKEN_REQUEST);
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTAdminService_processPreAuthTokenJNI(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jbyteArray content,
                                                                               jbyteArray user_pubkey,
                                                                               jbyteArray user_privkey) {
    jbyte *content_ptr;
    jsize content_len;
    loadJavaArray(env, content, content_ptr, content_len);

    AsymmetricEncryptionKeySet keySet = getUserKeySet(env, user_pubkey, user_privkey);

    auto request_msg = GetAcquirePreAuthTokenRequest(content_ptr);
    SignaturePreAuthToken token_content;
    memcpy(token_content.user_pubkey, request_msg->user_pubkey()->Data(),
           request_msg->user_pubkey()->Length());

    CiphertextAcquirePreAuthTokenResponse response_payload;
    response_payload.type = CipherType::acquire_pre_auth_token;

    keySet.sign_detached(response_payload.token,
                         reinterpret_cast<const unsigned char *>(&token_content),
                         sizeof(token_content));

    LOGV("Event: Acquire pre auth token admin grant");
    LOGV("Token content base64: %s",
         base64_encode(response_payload.token, sizeof(response_payload.token)).c_str());

    uint8_t ciphertext_buf_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(
            sizeof(response_payload));
    auto ciphertext_buf = new uint8_t[ciphertext_buf_len];
    uint8_t nonce[AsymmetricEncryptionKeySet::NONCE_SIZE];
    keySet.box_easy(ciphertext_buf, ciphertext_buf_len,
                    reinterpret_cast<const uint8_t *>(&response_payload),
                    sizeof(response_payload), nonce, request_msg->user_pubkey()->Data());

    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto nonce_obj = builder.CreateVector(nonce, sizeof(nonce));
    auto ciphertext_obj = builder.CreateVector(ciphertext_buf, ciphertext_buf_len);
    auto response_msg = CreateAcquirePreAuthTokenResponse(builder, nonce_obj, ciphertext_obj);
    builder.Finish(response_msg);

    uint8_t out[G_DATA_BUF_SIZE]{};
    int response_type_len = network_send_message_type(0,
                                                      MessageType_ACQUIRE_PRE_AUTH_TOKEN_RESPONSE,
                                                      out);
    int response_content_len = network_send(0, builder.GetBufferPointer(), builder.GetSize(),
                                            SOCKET_SEND_FLAGS, out + response_type_len);

    int response_len = response_type_len + response_content_len;
    jbyteArray ret = env->NewByteArray(response_len);
    env->SetByteArrayRegion(ret, 0, response_len,
                            reinterpret_cast<const jbyte *>(out));

    delete[] ciphertext_buf;

    return ret;
}extern "C"
JNIEXPORT jboolean JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_checkMessageTypeDataStoreSieveCredRequestJNI(
        JNIEnv *env, jobject thiz, jbyteArray message_type) {
    return checkMessageType(env, message_type, MessageType_DATA_STORE_SIEVE_CRED_REQUEST);
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_processSieveCredRequestJNI(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jbyteArray request_content,
                                                                                  jbyteArray user_pubkey,
                                                                                  jbyteArray user_privkey) {
    jbyte *request_content_ptr;
    jsize request_content_len;
    loadJavaArray(env, request_content, request_content_ptr, request_content_len);

    AsymmetricEncryptionKeySet keySet = getUserKeySet(env, user_pubkey, user_privkey);

    auto request_msg = GetDataStoreSieveCredRequest(request_content_ptr);

    CiphertextDataStoreSieveCredRequest request_payload;
    keySet.box_open_easy(reinterpret_cast<uint8_t *>(&request_payload), sizeof(request_payload),
                         request_msg->ciphertext()->Data(), request_msg->ciphertext()->Length(),
                         request_msg->session_nonce()->Data(),
                         request_msg->device_pubkey()->Data());

    if (request_payload.type != CipherType::data_store_sieve_cred_request) {
        LOGW("Unexpected cred request type");
        return nullptr;
    }

    SharedSecretKey session_key(request_payload.session_key, sizeof(request_payload.session_key));


    SieveKey sieve_key;

    CiphertextDataStoreSieveCredResponse response_payload;
    response_payload.type = CipherType::data_store_sieve_cred_response;
    sieve_key.serialize_key_into(response_payload.sieve_key, sizeof(response_payload.sieve_key));
    sieve_key.serialize_nonce_into(response_payload.sieve_nonce,
                                   sizeof(response_payload.sieve_nonce));

    size_t response_cipher_len = SharedSecretKey::get_cipher_len(sizeof(response_payload));
    auto response_cipher = new uint8_t[response_cipher_len]{};
    session_key.encrypt(response_cipher, response_cipher_len,
                        reinterpret_cast<uint8_t *>(&response_payload),
                        sizeof(response_payload));

    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto response_session_header_obj = builder.CreateVector(session_key.get_header(),
                                                            SharedSecretKey::HEADER_SIZE);
    auto response_cipher_obj = builder.CreateVector(response_cipher, response_cipher_len);
    auto response_msg = CreateDataStoreSieveCredResponse(builder, response_session_header_obj,
                                                         response_cipher_obj);
    builder.Finish(response_msg);


    jclass clazz = env->FindClass("me/zhanghan177/tot_mobile_native/TOTUserService");

    // Save Sieve Key
    uint8_t sieve_key_buf[SIEVE_KEY_SIZE]{};
    sieve_key.serialize_key_into(sieve_key_buf, sizeof(sieve_key_buf));
    jbyteArray sieve_key_buf_j = env->NewByteArray(sizeof(sieve_key_buf));
    env->SetByteArrayRegion(sieve_key_buf_j, 0, sizeof(sieve_key_buf),
                            reinterpret_cast<const jbyte *>(sieve_key_buf));

    jmethodID midSetSieveKey = env->GetMethodID(clazz, "setSieveKey", "([B)V");
    env->CallVoidMethod(thiz, midSetSieveKey, sieve_key_buf_j);

    uint8_t sieve_key_nonce[SIEVE_NONCE_SIZE]{};
    sieve_key.serialize_nonce_into(sieve_key_nonce, sizeof(sieve_key_nonce));
    jbyteArray sieve_key_nonce_j = env->NewByteArray(sizeof(sieve_key_nonce));
    env->SetByteArrayRegion(sieve_key_nonce_j, 0, sizeof(sieve_key_nonce),
                            reinterpret_cast<const jbyte *>(sieve_key_nonce));

    jmethodID midSetSieveKeyNonce = env->GetMethodID(clazz, "setSieveKeyNonce", "([B)V");
    env->CallVoidMethod(thiz, midSetSieveKeyNonce, sieve_key_nonce_j);

    // Save Session Key
    jbyteArray session_key_buf_j = env->NewByteArray(SharedSecretKey::KEY_SIZE);
    env->SetByteArrayRegion(session_key_buf_j, 0, SharedSecretKey::KEY_SIZE,
                            reinterpret_cast<const jbyte *>(session_key.get_key()));

    jmethodID midSetSessionKey = env->GetMethodID(clazz, "setSessionKey", "([B)V");
    env->CallVoidMethod(thiz, midSetSessionKey, session_key_buf_j);


    uint8_t out[G_DATA_BUF_SIZE]{};
    int response_type_len = network_send_message_type(0, MessageType_DATA_STORE_SIEVE_CRED_RESPONSE,
                                                      out);
    int response_content_len = network_send(0, builder.GetBufferPointer(), builder.GetSize(),
                                            SOCKET_SEND_FLAGS, out + response_type_len);

    int response_len = response_type_len + response_content_len;
    jbyteArray ret = env->NewByteArray(response_len);
    env->SetByteArrayRegion(ret, 0, response_len,
                            reinterpret_cast<const jbyte *>(out));

    delete[] response_cipher;

    return ret;
}extern "C"
JNIEXPORT jboolean JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_checkMessageTypeUploadNotificationJNI(
        JNIEnv *env, jobject thiz, jbyteArray message_type) {
    return checkMessageType(env, message_type, MessageType_DATA_STORE_UPLOAD_NOTIFICATION);
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_processUploadNotificationJNI(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jbyteArray request_content,
                                                                                    jbyteArray user_pubkey,
                                                                                    jbyteArray user_privkey,
                                                                                    jbyteArray session_key_in) {
    jbyte *request_content_ptr;
    jsize request_content_len;
    loadJavaArray(env, request_content, request_content_ptr, request_content_len);

    AsymmetricEncryptionKeySet keySet = getUserKeySet(env, user_pubkey, user_privkey);

    jbyte *session_key_ptr;
    jsize session_key_len;
    loadJavaArray(env, session_key_in, session_key_ptr, session_key_len);

    SharedSecretKey session_key(reinterpret_cast<const uint8_t *>(session_key_ptr),
                                session_key_len);

    auto notification_msg = DataStoreUpload::GetDataStoreUploadNotification(request_content_ptr);

    session_key.load_header_decryption(notification_msg->session_header_refresh()->Data(),
                                       notification_msg->session_header_refresh()->Length());
    CiphertextDataStoreUploadNotification notification_payload;
    session_key.decrypt(reinterpret_cast<uint8_t *>(&notification_payload),
                        sizeof(notification_payload),
                        notification_msg->ciphertext()->Data(),
                        notification_msg->ciphertext()->Length());

    if (notification_payload.type != CipherType::data_store_upload_notification) {
        LOGW("Unexpected data store notification type");
        return nullptr;
    }

    jbyteArray ret = env->NewByteArray(sizeof(notification_payload.sieve_data_block_uuid));
    env->SetByteArrayRegion(ret, 0,
                            sizeof(notification_payload.sieve_data_block_uuid),
                            notification_payload.sieve_data_block_uuid);

    return ret;
}extern "C"
JNIEXPORT jboolean JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_checkMessageTypeDataAccessFetchJNI(
        JNIEnv *env, jobject thiz, jbyteArray message_type) {
    return checkMessageType(env, message_type, MessageType_DATA_ACCESS_FETCH);
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_processDataAccessFetchJNI(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jbyteArray request_content,
                                                                                 jbyteArray user_pubkey,
                                                                                 jbyteArray user_privkey) {
    jbyte *request_content_ptr;
    jsize request_content_len;
    loadJavaArray(env, request_content, request_content_ptr, request_content_len);

    AsymmetricEncryptionKeySet keySet = getUserKeySet(env, user_pubkey, user_privkey);
    jclass clazz = env->FindClass("me/zhanghan177/tot_mobile_native/TOTUserService");

    auto fetch_msg = GetDataAccessFetch(request_content_ptr);

    CiphertextDataAccessFetch fetch_payload;
    keySet.box_open_easy(reinterpret_cast<uint8_t *>(&fetch_payload), sizeof(fetch_payload),
                         fetch_msg->ciphertext()->data(), fetch_msg->ciphertext()->size(),
                         fetch_msg->message_nonce()->data(), fetch_msg->accessor_pubkey()->data());

    if (fetch_payload.type != CipherType::data_access_fetch) {
        LOGW("Wrong fetch request type");
        return nullptr;
    }

    UUID sieve_uuid(fetch_payload.sieve_data_block_uuid,
                    sizeof(fetch_payload.sieve_data_block_uuid));

//    if (!delegate_access(sieve_uuid,
//                         fetch_msg->accessor_pubkey()->data(),
//                         fetch_msg->accessor_pubkey()->size()))
//    {
//        LOGI("Access denied");
//        return -1;
//    }

//    if (sieve_key_table.find(sieve_uuid) == sieve_key_table.end())
//    {
//        LOGW("Sieve uuid not found: %s", sieve_uuid.get_uuid().c_str());
//        return -1;
//    }

    uint8_t sieve_key_buf[SIEVE_KEY_SIZE]{};

    jmethodID midGetSieveKey = env->GetMethodID(clazz, "getSieveKey", "()[B");
    jbyteArray sieve_key_buf_j = static_cast<jbyteArray>(env->CallObjectMethod(thiz,
                                                                               midGetSieveKey));
    env->GetByteArrayRegion(sieve_key_buf_j, 0, sizeof(sieve_key_buf),
                            reinterpret_cast<jbyte *>(sieve_key_buf));

    uint8_t sieve_key_nonce[SIEVE_NONCE_SIZE]{};
    jmethodID midGetSieveKeyNonce = env->GetMethodID(clazz, "getSieveKeyNonce", "()[B");
    jbyteArray sieve_key_nonce_j = static_cast<jbyteArray>(env->CallObjectMethod(thiz,
                                                                                 midGetSieveKeyNonce));
    env->GetByteArrayRegion(sieve_key_nonce_j, 0, sizeof(sieve_key_nonce),
                            reinterpret_cast<jbyte *>(sieve_key_nonce));

    SieveKey sieveKey(sieve_key_buf, sizeof(sieve_key_buf),
                      sieve_key_nonce, sizeof(sieve_key_nonce));

    CiphertextDataAccessResponse response;
    response.type = CipherType::data_access_response;
    sieveKey.serialize_key_into(response.sieve_key, sizeof(response.sieve_key));
    memcpy(response.random_challenge_response,
           fetch_payload.random_challenge,
           sizeof(fetch_payload.random_challenge));

    uint8_t msg_nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{};

    size_t cipher_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(response));
    auto cipher = new uint8_t[cipher_len]{};
    keySet.box_easy(cipher, cipher_len, reinterpret_cast<const uint8_t *>(&response),
                    sizeof(response), msg_nonce, fetch_msg->accessor_pubkey()->data());

    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto msg_nonce_obj = builder.CreateVector(msg_nonce, sizeof(msg_nonce));
    auto ciphertext_obj = builder.CreateVector(cipher, cipher_len);
    auto response_msg = CreateDataAccessResponse(builder, msg_nonce_obj, ciphertext_obj);
    builder.Finish(response_msg);

    uint8_t out[G_DATA_BUF_SIZE]{};
    int response_type_len = network_send_message_type(0, MessageType_DATA_ACCESS_RESPONSE,
                                                      out);
    int response_content_len = network_send(0, builder.GetBufferPointer(), builder.GetSize(),
                                            SOCKET_SEND_FLAGS, out + response_type_len);

    int response_len = response_type_len + response_content_len;
    jbyteArray ret = env->NewByteArray(response_len);
    env->SetByteArrayRegion(ret, 0, response_len,
                            reinterpret_cast<const jbyte *>(out));

    return ret;
}
//extern "C"
//JNIEXPORT void JNICALL
//Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_reencryptJNI(JNIEnv *env, jobject thiz,
//                                                                        jbyteArray user_pubkey_in,
//                                                                        jbyteArray user_privkey_in,
//                                                                        jstring uuid_in,
//                                                                        jstring enc_meta_block_uuid_in,
//                                                                        jbyteArray sieve_key_j,
//                                                                        jbyteArray sieve_key_nonce_j,
//                                                                        jstring storage_ip_in,
//                                                                        jint storage_port_in) {

extern "C"
JNIEXPORT void JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_reencryptJNI(JNIEnv *env, jobject thiz,
                                                                        jbyteArray user_pubkey_in,
                                                                        jbyteArray user_privkey_in,
                                                                        jstring uuid_in,
                                                                        jstring enc_meta_block_uuid_in,
                                                                        jbyteArray sieve_key_j,
                                                                        jbyteArray sieve_key_nonce_j,
                                                                        jstring storage_ip_in,
                                                                        jint storage_port_in) {
    jbyte *user_pubkey_ptr;
    jsize user_pubkey_len;
    loadJavaArray(env, user_pubkey_in, user_pubkey_ptr, user_pubkey_len);

    jbyte *user_privkey_ptr;
    jsize user_privkey_len;
    loadJavaArray(env, user_privkey_in, user_privkey_ptr, user_privkey_len);

    AsymmetricEncryptionKeySet keySet(
            reinterpret_cast<const uint8_t *>(user_pubkey_ptr), user_pubkey_len,
            reinterpret_cast<const uint8_t *>(user_privkey_ptr), user_privkey_len);


    const char *uuid_load = env->GetStringUTFChars(uuid_in, nullptr);
    libtot::UUID sieve_data_block_uuid(uuid_load, libtot::UUID::UUID_SIZE);

    const char *enc_meta_block_uuid_load = env->GetStringUTFChars(enc_meta_block_uuid_in, nullptr);

    const char *storage_ip_load = env->GetStringUTFChars(storage_ip_in, nullptr);

    jbyte *sieve_key_ptr;
    jsize sieve_key_len;
    loadJavaArray(env, sieve_key_j, sieve_key_ptr, sieve_key_len);

    jbyte *sieve_key_nonce_ptr;
    jsize sieve_key_nonce_len;
    loadJavaArray(env, sieve_key_nonce_j, sieve_key_nonce_ptr, sieve_key_nonce_len);

    SieveKey orig_sieve_key(reinterpret_cast<const uint8_t *>(sieve_key_ptr), sieve_key_len,
                            reinterpret_cast<const uint8_t *>(sieve_key_nonce_ptr),
                            sieve_key_nonce_len);

    /**
     * Main body for re-encryption
     */

    SieveKey sieve_key_new;
    RekeyToken token = orig_sieve_key.gen_rekey_token(sieve_key_new);

    uint8_t *dynamic_pk = nullptr;
    // Fetch storage's public key through out-of-band trusted KMS
    int conn_temp = network_connect(storage_ip_load, storage_port_in);
    network_send_message_type(conn_temp, MessageType_UTIL_FETCH_STORE_PUBKEY);
    uint8_t store_pk_buf[READ_BUFFER_SIZE]{};
    network_read(conn_temp, store_pk_buf, sizeof(store_pk_buf));
    auto msg = GetUtilFetchStorePubkey(store_pk_buf);
    dynamic_pk = new uint8_t[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
    memcpy(dynamic_pk, msg->pubkey()->data(), AsymmetricEncryptionKeySet::FULL_PK_SIZE);


    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    size_t cipher_len = 0;
    uint8_t *cipher = nullptr;
    int conn = network_connect(storage_ip_load, storage_port_in);

    /**
     * Negotiate pre-request to prevent replay attack
     */
    // Send pre request
    CiphertextDataReencryptionPreRequest pre_req_payload;
    pre_req_payload.type = CipherType::data_reencryption_pre_request;
    memcpy(pre_req_payload.sieve_data_block_uuid,
           sieve_data_block_uuid.get_uuid().c_str(),
           sizeof(pre_req_payload.sieve_data_block_uuid));
    memcpy(pre_req_payload.encrypted_metadata_uuid,
           enc_meta_block_uuid_load,
           sizeof(pre_req_payload.encrypted_metadata_uuid));
    random_buf(pre_req_payload.user_nonce, sizeof(pre_req_payload.user_nonce));

    cipher_len = AsymmetricEncryptionKeySet::get_box_seal_cipher_len(sizeof(pre_req_payload));
    cipher = new uint8_t[cipher_len]{0};

    keySet.box_seal(cipher, cipher_len,
                    reinterpret_cast<const uint8_t *>(&pre_req_payload),
                    sizeof(pre_req_payload), dynamic_pk);

    builder.Clear();
    auto pre_req_cipher_obj = builder.CreateVector(cipher, cipher_len);
    auto pre_req_msg = CreateDataReencryptionPreRequest(builder, pre_req_cipher_obj);
    builder.Finish(pre_req_msg);

    network_send_message_type(conn, MessageType_DATA_REENCRYPTION_PRE_REQUEST);
    network_send(conn, builder.GetBufferPointer(), builder.GetSize());

    // Process pre response
    if (network_read_message_type(conn) != MessageType_DATA_REENCRYPTION_PRE_RESPONSE) {
        LOGW("Unexpected message for Pre-response!");
        return;
    }
    uint8_t pre_res_buf[READ_BUFFER_SIZE]{0};
    network_read(conn, pre_res_buf, sizeof(pre_res_buf));
    auto pre_res_msg = GetDataReencryptionPreResponse(pre_res_buf);
    CiphertextDataReencryptionPreResponse pre_res_payload;
    keySet.box_seal_open(reinterpret_cast<uint8_t *>(&pre_res_payload),
                         sizeof(pre_res_payload),
                         pre_res_msg->ciphertext()->data(),
                         pre_res_msg->ciphertext()->size());

    if (pre_res_payload.type != CipherType::data_reencryption_pre_response) {
        LOGW("Wrong message type for pre response");
        return;
    }

    if (memcmp(pre_res_payload.user_nonce,
               pre_req_payload.user_nonce,
               sizeof(pre_res_payload.user_nonce)) != 0) {
        LOGW("Incorrect user nonce responded");
        return;
    }

    /**
     * Construct the main request
     */
    CiphertextDataReencryptionRequest request_payload;
    request_payload.type = CipherType::data_reencryption_request;
    memcpy(&(request_payload.rekey_token),
           reinterpret_cast<const uint8_t *>(&token),
           sizeof(token));
    random_buf(request_payload.noti_token, G_CHALLENGE_SIZE);
    memcpy(request_payload.user_nonce,
           pre_req_payload.user_nonce,
           sizeof(request_payload.user_nonce));
    memcpy(request_payload.storage_nonce,
           pre_res_payload.storage_nonce,
           sizeof(request_payload.storage_nonce));

    cipher_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(request_payload));
    delete[] cipher;
    cipher = new uint8_t[cipher_len]{0};
    uint8_t nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{0};
    keySet.box_easy(cipher, cipher_len,
                    reinterpret_cast<const uint8_t *>(&request_payload),
                    sizeof(request_payload), nonce, dynamic_pk);

    builder.Clear();
    auto sieve_uuid_obj = builder.CreateString(sieve_data_block_uuid.get_uuid());
    auto owner_pk_obj = builder.CreateVector(reinterpret_cast<const uint8_t *>(user_pubkey_ptr),
                                             sizeof(user_pubkey_ptr));
    auto msg_nonce_obj = builder.CreateVector(nonce, sizeof(nonce));
    auto cipher_obj = builder.CreateVector(cipher, cipher_len);
    auto request_msg = CreateDataReencryptionRequest(builder, sieve_uuid_obj, owner_pk_obj,
                                                     msg_nonce_obj, cipher_obj);
    builder.Finish(request_msg);

    network_send_message_type(conn, MessageType_DATA_REENCRYPTION_REQUEST);
    network_send(conn, builder.GetBufferPointer(), builder.GetSize());

    if (network_read_message_type(conn) != MessageType_DATA_REENCRYPTION_RESPONSE) {
        LOGW("Wrong reencryption response message type");
        return;
    }
    uint8_t res_buf[READ_BUFFER_SIZE]{0};
    network_read(conn, res_buf, sizeof(res_buf));
    auto res_msg = GetDataReencryptionResponse(res_buf);

    if (memcmp(res_msg->notification_token()->data(),
               request_payload.noti_token,
               sizeof(request_payload.noti_token)) != 0) {
        LOGW("Unmatched notification token!");
        return;
    }

    delete[] cipher;
    delete[] dynamic_pk;


}
extern "C"
JNIEXPORT jboolean JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_checkMessageTypeUtilRealTimeAccessRequestJNI(
        JNIEnv *env, jobject thiz, jbyteArray message_type) {
    return checkMessageType(env, message_type, MessageType_UTIL_REAL_TIME_ACCESS_REQUEST);
}
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTUserService_processUtilRealTimeAccessJNI(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jboolean granted) {
    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto res_msg = CreateUtilRealTimeAccessResponse(builder, granted);
    builder.Finish(res_msg);

    uint8_t out[G_DATA_BUF_SIZE]{};
    int response_type_len = network_send_message_type(0,
                                                      MessageType_UTIL_REAL_TIME_ACCESS_RESPONSE,
                                                      out);
    int response_content_len = network_send(0, builder.GetBufferPointer(), builder.GetSize(),
                                            SOCKET_SEND_FLAGS, out + response_type_len);

    int response_len = response_type_len + response_content_len;
    jbyteArray ret = env->NewByteArray(response_len);
    env->SetByteArrayRegion(ret, 0, response_len,
                            reinterpret_cast<const jbyte *>(out));

    return ret;
}
extern "C"
JNIEXPORT void JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_releaseDeviceJNI(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jbyteArray claimed_device_in,
                                                                            jstring claimed_device_ip_in,
                                                                            jint claimed_device_port,
                                                                            jbyteArray user_pubkey) {
    jbyte *claimed_device_ptr;
    jsize claimed_device_len;
    loadJavaArray(env, claimed_device_in, claimed_device_ptr, claimed_device_len);

    const char *device_ip_load = env->GetStringUTFChars(claimed_device_ip_in, nullptr);

    jbyte *user_pubkey_ptr;
    jsize user_pubkey_len;
    loadJavaArray(env, user_pubkey, user_pubkey_ptr, user_pubkey_len);


    int device_conn = network_connect(device_ip_load, claimed_device_port);

    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto pubkey_obj = builder.CreateVector(reinterpret_cast<const uint8_t *>(user_pubkey_ptr),
                                           user_pubkey_len);
    auto release_msg = CreateUtilReleaseDevice(builder, pubkey_obj);
    builder.Finish(release_msg);

    network_send_message_type(device_conn, MessageType_UTIL_RELEASE_DEVICE);
    network_send(device_conn, builder.GetBufferPointer(), builder.GetSize());
}

extern "C"
JNIEXPORT void JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_removeRealTimeAccessJNI(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jbyteArray claimed_device_in,
                                                                                   jstring claimed_device_ip_in,
                                                                                   jint claimed_device_port,
                                                                                   jbyteArray user_pubkey) {
    jbyte *claimed_device_ptr;
    jsize claimed_device_len;
    loadJavaArray(env, claimed_device_in, claimed_device_ptr, claimed_device_len);

    const char *device_ip_load = env->GetStringUTFChars(claimed_device_ip_in, nullptr);

    jbyte *user_pubkey_ptr;
    jsize user_pubkey_len;
    loadJavaArray(env, user_pubkey, user_pubkey_ptr, user_pubkey_len);


    int device_conn = network_connect(device_ip_load, claimed_device_port);

    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto pubkey_obj = builder.CreateVector(reinterpret_cast<const uint8_t *>(user_pubkey_ptr),
                                           user_pubkey_len);
    auto release_msg = CreateUtilRemoveRealTimeAccess(builder, pubkey_obj);
    builder.Finish(release_msg);

    network_send_message_type(device_conn, MessageType_UTIL_REMOVE_REAL_TIME_ACCESS);
    network_send(device_conn, builder.GetBufferPointer(), builder.GetSize());
}
extern "C"
JNIEXPORT void JNICALL
Java_me_zhanghan177_tot_1mobile_1native_TOTKeyStoreService_proximityHeartbeatJNI(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jbyteArray claimed_device_in,
                                                                                 jstring claimed_device_ip_in,
                                                                                 jint claimed_device_port,
                                                                                 jbyteArray proximity_nonce_in,
                                                                                 jbyteArray user_pubkey) {
    jbyte *claimed_device_ptr;
    jsize claimed_device_len;
    loadJavaArray(env, claimed_device_in, claimed_device_ptr, claimed_device_len);

    const char *device_ip_load = env->GetStringUTFChars(claimed_device_ip_in, nullptr);

    jbyte *proximity_nonce_ptr;
    jsize proximity_nonce_len;
    loadJavaArray(env, proximity_nonce_in, proximity_nonce_ptr, proximity_nonce_len);

    jbyte *user_pubkey_ptr;
    jsize user_pubkey_len;
    loadJavaArray(env, user_pubkey, user_pubkey_ptr, user_pubkey_len);

    int device_conn = network_connect(device_ip_load, claimed_device_port);

    flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
    auto proximity_nonce_obj = builder.CreateVector(
            reinterpret_cast<const uint8_t *>(proximity_nonce_ptr), proximity_nonce_len);
    auto pubkey_obj = builder.CreateVector(reinterpret_cast<const uint8_t *>(user_pubkey_ptr),
                                           user_pubkey_len);
    auto heartbeat_msg = CreateUtilHeartbeat(builder, pubkey_obj, proximity_nonce_obj);
    builder.Finish(heartbeat_msg);

    network_send_message_type(device_conn, MessageType_UTIL_HEARTBEAT);
    network_send(device_conn, builder.GetBufferPointer(), builder.GetSize());
}