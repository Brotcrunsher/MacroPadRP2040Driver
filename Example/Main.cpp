#include <windows.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <stdio.h>

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

int main() {
    GUID hidGuid;
    HidD_GetHidGuid(&hidGuid);
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        printf("Error: Couldn't find deviceInfoSet!\n");
        return 1;
    }

    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    DWORD index = 0;
    bool deviceFound = false;

    while (SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &hidGuid, index, &deviceInterfaceData)) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);

        PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        deviceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceDetail, requiredSize, NULL, NULL)) {
            HANDLE deviceHandle = CreateFile(deviceDetail->DevicePath, GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

            if (deviceHandle != INVALID_HANDLE_VALUE) {
                HIDD_ATTRIBUTES attributes;
                attributes.Size = sizeof(HIDD_ATTRIBUTES);
                if (HidD_GetAttributes(deviceHandle, &attributes)) {
                    printf("Found device - VID: %04X, PID: %04X\n", attributes.VendorID, attributes.ProductID);

                    if (attributes.VendorID == 0x239A /* Adafruit VID */ && attributes.ProductID == 0x8108 /* MacroPad PID */) {
                        printf("Found MacroPad!\n");
                        deviceFound = true;

                        PHIDP_PREPARSED_DATA preparsedData;
                        if (HidD_GetPreparsedData(deviceHandle, &preparsedData)) {
                            HIDP_CAPS caps;
                            if (HidP_GetCaps(preparsedData, &caps) == HIDP_STATUS_SUCCESS) {
                                printf("UsagePage: %04X, Usage: %04X\n", caps.UsagePage, caps.Usage);

                                DWORD bytesRead;
                                BYTE* report = new BYTE[caps.InputReportByteLength];

                                while (true) {
                                    OVERLAPPED overlapped = { 0 };
                                    overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
                                    BOOL result = ReadFile(deviceHandle, report, caps.InputReportByteLength, &bytesRead, &overlapped);
                                    if (!result) {
                                        if (GetLastError() == ERROR_IO_PENDING) {
                                            WaitForSingleObject(overlapped.hEvent, INFINITE);
                                            GetOverlappedResult(deviceHandle, &overlapped, &bytesRead, FALSE);
                                        }
                                        else {
                                            printf("Error: Could not read report.\n");
                                            CloseHandle(overlapped.hEvent);
                                            break;
                                        }
                                    }

                                    if (bytesRead > 0) {
                                        for(size_t i = 0; i<bytesRead; i++) printf("%d ", report[i]);
                                        printf("\n");
                                    }
                                    CloseHandle(overlapped.hEvent);
                                }

                                delete[] report;
                            }
                            HidD_FreePreparsedData(preparsedData);
                        }

                        CloseHandle(deviceHandle);
                        free(deviceDetail);
                        break;
                    }
                }
                CloseHandle(deviceHandle);
            }
        }
        free(deviceDetail);
        index++;
    }

    if (!deviceFound) {
        printf("Error: Could not find MacroPad.\n");
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return 0;
}
