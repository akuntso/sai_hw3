#include <stdio.h>
#include "sai.h"

const char* test_profile_get_value(
    _In_ sai_switch_profile_id_t profile_id,
    _In_ const char* variable)
{
    return 0;
}

int test_profile_get_next_value(
    _In_ sai_switch_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

int main()
{
    sai_status_t              status;
    sai_switch_api_t         *switch_api;
    sai_object_id_t           vr_oid;
    sai_switch_notification_t notifications;
    sai_lag_api_t            *lag_api;
    sai_object_id_t           lag_oid1, lag_oid2, lag_oid3, lag_oid4, lag_oid5, lag_oid6; 
    sai_object_id_t           lag_member_oid1, lag_member_oid2, lag_member_oid3, lag_member_oid4, lag_member_oid5, lag_member_oid6, lag_member_oid7, lag_member_oid8, lag_member_oid9, lag_member_oid10, lag_member_oid11, lag_member_oid12, lag_member_oid13, lag_member_oid14, lag_member_oid15, lag_member_oid16, lag_member_oid17, lag_member_oid18;
    sai_attribute_t           lag_attr;
    sai_attribute_t           attrs[2];
    sai_attribute_t           lag_member_attr[2];
    sai_object_id_t           port_list[32];
    

    status = sai_api_initialize(0, &test_services);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to initialize SAI API, status=%d\n", status);
        return 1;
    }

    status = sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
    status = switch_api->initialize_switch(0, "HW_ID", 0, &notifications);
    attrs[0].id = SAI_SWITCH_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = port_list;
    attrs[0].value.objlist.count = 64;
    
    status = switch_api->get_switch_attribute(1, attrs);

    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query LAG API, status=%d\n", status);
        return 1;
    }

    // Create LAG #1
    status = lag_api->create_lag(&lag_oid1, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG #1, status=%d\n", status);
        return 1;
    }

    // Create LAG #2
    status = lag_api->create_lag(&lag_oid2, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG #2, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER #1 {LAG_ID:LAG#1, PORT_ID:PORT#1}
    lag_member_attr[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    lag_member_attr[0].value.oid = lag_oid1;
    lag_member_attr[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    lag_member_attr[1].value.oid = port_list[0];
    status = lag_api->create_lag_member(&lag_member_oid1, 2, lag_member_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER #1, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER #2 {LAG_ID:LAG#1, PORT_ID:PORT#2}
    lag_member_attr[0].value.oid = lag_oid1;
    lag_member_attr[1].value.oid = port_list[1];
    status = lag_api->create_lag_member(&lag_member_oid2, 2, lag_member_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER #2, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER #3 {LAG_ID:LAG#2, PORT_ID:PORT#3}
    lag_member_attr[0].value.oid = lag_oid2;
    lag_member_attr[1].value.oid = port_list[2];
    status = lag_api->create_lag_member(&lag_member_oid3, 2, lag_member_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER #3, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER #4 {LAG_ID:LAG#2, PORT_ID:PORT#4}
    lag_member_attr[0].value.oid = lag_oid2;
    lag_member_attr[1].value.oid = port_list[3];
    status = lag_api->create_lag_member(&lag_member_oid4, 2, lag_member_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER #4, status=%d\n", status);
        return 1;
    }

    lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
    status = lag_api->get_lag_attribute(lag_oid1, 1, &lag_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG #1 attributes, status=%d\n", status);
        return 1;
    }

    lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
    status = lag_api->get_lag_attribute(lag_oid2, 1, &lag_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG #2 attributes, status=%d\n", status);
        return 1;
    }

    lag_member_attr[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    status = lag_api->get_lag_member_attribute(lag_member_oid1, 1, lag_member_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG_MEMBER #1 attributes, status=%d\n", status);
        return 1;
    }

    lag_member_attr[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    status = lag_api->get_lag_member_attribute(lag_member_oid3, 1, lag_member_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG_MEMBER #3 attributes, status=%d\n", status);
        return 1;
    }

    // Remove LAG_MEMBER #2
    status = lag_api->remove_lag_member(lag_member_oid2);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER #2, status=%d\n", status);
        return 1;
    }

    lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
    status = lag_api->get_lag_attribute(lag_oid1, 1, &lag_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG #1 attributes after removing member, status=%d\n", status);
        return 1;
    }

    // Remove LAG_MEMBER #3
    status = lag_api->remove_lag_member(lag_member_oid3);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER #3, status=%d\n", status);
        return 1;
    }

    lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
    status = lag_api->get_lag_attribute(lag_oid2, 1, &lag_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG #2 attributes after removing member, status=%d\n", status);
        return 1;
    }

    // Remove LAG_MEMBER #1
    status = lag_api->remove_lag_member(lag_member_oid1);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER #1, status=%d\n", status);
        return 1;
    }

    // Remove LAG_MEMBER #4
    status = lag_api->remove_lag_member(lag_member_oid4);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER #4, status=%d\n", status);
        return 1;
    }

    // Remove LAG #2
    status = lag_api->remove_lag(lag_oid2);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG #2, status=%d\n", status);
        return 1;
    }

    // Remove LAG #1
    status = lag_api->remove_lag(lag_oid1);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG #1, status=%d\n", status);
        return 1;
    }

    status = sai_api_uninitialize();
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to uninitialize SAI API, status=%d\n", status);
        return 1;
    }
    // Create 6 LAGs (System should return error - Cannot create LAG: limit is reached)
    sai_object_id_t lag_oids[6];

    for (int i = 0; i < 6; i++) {
        status = lag_api->create_lag(&lag_oids[i], 0, NULL);
    }

    // Create 17 LAG_MEMBERs (System should return error - Cannot create LAG MEMBER: limit is reached)
    sai_object_id_t lag_member_oids[17];
    sai_attribute_t           lag_member_attrs[2];
    lag_member_attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    lag_member_attrs[0].value.oid = lag_oid1;
    lag_member_attrs[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    lag_member_attrs[1].value.oid = port_list[0];

    for (int i = 0; i < 17; i++) {
        status = lag_api->create_lag_member(&lag_member_oids[i], 2, lag_member_attrs);
    }

    // Remove non-existent LAG_MEMBER #18
    status = lag_api->remove_lag_member(lag_member_oid18);
    printf("Failed to remove LAG_MEMBER #18, status=%d\n", status);
   
    // Remove non-existent LAG #5
    status = lag_api->remove_lag(lag_oid5);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG #5, status=%d\n", status);
        return 1;
    }

    return 0;

}