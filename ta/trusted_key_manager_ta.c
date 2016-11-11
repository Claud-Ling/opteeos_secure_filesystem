#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "trusted_key_manager_ta.h"

TEE_Result TA_CreateEntryPoint(void)
{
  return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

static TEE_Result encrypt_value(uint32_t param_types, TEE_Param params[4]) {
  unsigned char key1[16], key2[16], tweak[16];
  unsigned char *data, *data_temp;
  unsigned int data_len;
  TEE_OperationHandle handle;
  TEE_Result allocate_result, set_key_result, cipher_result;
  TEE_ObjectHandle keyObj1, keyObj2;
  TEE_Attribute keyAttr1Arr[1];
  TEE_Attribute keyAttr2Arr[1];

  uint32_t expected_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                                            TEE_PARAM_TYPE_MEMREF_INOUT,
                                            TEE_PARAM_TYPE_NONE,
                                            TEE_PARAM_TYPE_NONE);

  if(param_types != expected_types){
    return TEE_ERROR_BAD_PARAMETERS;
  }

  if(params[0].memref.size < 48){
    //TODO: different return code
    return TEE_ERROR_BAD_PARAMETERS;
  }

  data = params[1].memref.buffer;
  data_len = params[1].memref.size;

  TEE_MemMove(key1, (unsigned char*)(params[0].memref.buffer), 16);
  TEE_MemMove(key2, (unsigned char*)(params[0].memref.buffer) + 16, 16);
  TEE_MemMove(tweak, (unsigned char*)(params[0].memref.buffer) + 32, 16);

  TEE_InitRefAttribute(keyAttr1Arr, TEE_ATTR_SECRET_VALUE, key1, 16);
  TEE_InitRefAttribute(keyAttr2Arr, TEE_ATTR_SECRET_VALUE, key2, 16);

  if((allocate_result = TEE_AllocateOperation(&handle, TEE_ALG_AES_XTS, TEE_MODE_ENCRYPT, 128)) != TEE_SUCCESS){
    return allocate_result;
  }

  if((allocate_result = TEE_AllocateTransientObject(TEE_TYPE_AES, 128, &keyObj1)) != TEE_SUCCESS) {
      return allocate_result;
  }

  if((allocate_result = TEE_AllocateTransientObject(TEE_TYPE_AES, 128, &keyObj2)) != TEE_SUCCESS) {
      return allocate_result;
  }

  if((allocate_result = TEE_PopulateTransientObject(keyObj1, keyAttr1Arr, 1)) != TEE_SUCCESS){
    return allocate_result;
  }

  if((allocate_result = TEE_PopulateTransientObject(keyObj2, keyAttr2Arr, 1)) != TEE_SUCCESS){
    return allocate_result;
  }


  if((set_key_result = TEE_SetOperationKey2(handle, keyObj1, keyObj2)) != TEE_SUCCESS){
    return set_key_result;
  }

  data_temp = TEE_Malloc(data_len, TEE_MALLOC_FILL_ZERO);

  TEE_CipherInit(handle, tweak, 16);
  if((cipher_result = TEE_CipherUpdate(handle, data, data_len, data_temp, &data_len)) != TEE_SUCCESS){
    TEE_Free(data_temp);
    return cipher_result;
  }

  if((cipher_result = TEE_CipherDoFinal(handle, NULL, 0, NULL, 0)) != TEE_SUCCESS){
    TEE_Free(data_temp);
    return cipher_result;
  }

  TEE_MemMove(data, data_temp, data_len);
  TEE_Free(data_temp);

  return TEE_SUCCESS;
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param  params[4], void **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case ENCRYPT_VALUE:
		return encrypt_value(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
