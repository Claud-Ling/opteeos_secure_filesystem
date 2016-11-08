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

static TEE_RESULT encrypt_value(uint32_t param_types, TEE_Param params[4]) {
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

  unsigned char key1[16];
  unsigned char key2[16];
  unsigned char tweak[16];
  unsigned char data = params[1].memref.buffer;
  unsigned int data_len = params[1].memref.size;
  unsigned char *data_temp;

  TEE_MemMove(key1, params[0].memref.buffer, 16);
  TEE_MemMove(key2, params[0].memref.buffer + 16, 16);
  TEE_MemMove(tweak, params[0].memref.buffer + 32, 16);

  TEE_OperationHandle handle;
  TEE_Result allocate_result, set_key_result, cipher_result;
  if((allocate_result = TEE_AllocateOperation(&handle, TEE_ALG_AES_XTS, TEE_MODE_ENCRYPT, 128)) != TEE_Success){
    return allocate_result;
  }

  if((set_key_result = TEE_SetOperationKey2(&handle, key1, key2)) != TEE_Success){
    return set_key_result;
  }

  data_temp = TEE_Malloc(data_len, TEE_MALLOC_FILL_ZERO);

  TEE_CipherInit(&handle, tweak, 16);
  if((cipher_result = TEE_CipherUpdate(&handle, data, data_len, data_temp, data_len)) != TEE_Success){
    TEE_Free(data_temp);
    return cipher_result;
  }

  if((cipher_result = TEE_CipherDoFinal(&handle, NULL, 0, NULL, 0)) != TEE_Success){
    TEE_Free(data_temp);
    return cipher_result;
  }

  TEE_MemMove(data, data_temp, data_len);
  TEE_Free(data_temp);

  return TEE_Success;
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
		return encrypt_values(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
