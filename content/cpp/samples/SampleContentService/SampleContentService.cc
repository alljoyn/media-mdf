/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include "SampleContentService.h"
#include "SampleContentSource.h"

#include "ContentServerBusObject.h"
#include "ContentBusListener.h"
#include "ContentSessionPortListener.h"
#include <ContentConstants.h>
#include <LogModule.h>
#include <alljoyn/Status.h>
#include <alljoyn/about/AboutServiceApi.h>


namespace mdf {

SampleContentService::SampleContentService()
{
    QCC_DbgTrace(("SampleContentService"));
    m_BusAttachment = NULL;
    m_sessionOpts = NULL;
    m_AboutPropertyStore = NULL;
    m_ContentBusListener = NULL;
    m_MdfContentSource = NULL;
}

SampleContentService::~SampleContentService()
{
    if (m_AboutService) {
        AboutServiceApi::DestroyInstance();
        m_AboutService = 0;
    }

    delete m_ContentServerBusObject;
    if (m_sessionOpts) {
        delete m_sessionOpts;
    }
    if (m_AboutPropertyStore) {
        delete m_AboutPropertyStore;
    }
    if (m_ContentBusListener) {
        delete m_ContentBusListener;
    }
    if (m_MdfContentSource) {
        delete m_MdfContentSource;
    }
    if (m_BusAttachment) {
        delete m_BusAttachment;
    }
}

QStatus SampleContentService::InitAboutService(BusAttachment* busAttachment, SessionPort portNumber)
{
    QCC_DbgTrace(("InitAboutService"));

    m_AboutPropertyStore = CreateAboutPropertyStore();
    if (m_AboutPropertyStore == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("Create Property store failed"));
        return ER_INIT_FAILED;
    }

    AboutServiceApi::Init(*busAttachment, *m_AboutPropertyStore);

    QCC_DbgTrace(("m_AboutService->Register"));
    m_AboutService = AboutServiceApi::getInstance();
    if (m_AboutService == NULL) {
        QCC_LogError(ER_BUS_NOT_ALLOWED, ("Failed to initialise the about service"));
        return ER_BUS_NOT_ALLOWED;
    }

    //now register the  port number with the about service and then register the about service
    QCC_DbgTrace(("m_AboutService->Register"));
    QStatus status = m_AboutService->Register(portNumber); //Could be a different port number. Could rename a differnt port - application needs to send this number.
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to register the port! Port: %d", portNumber));
        return status;
    }

    std::vector<qcc::String> mdfInterfaces;
    mdfInterfaces.push_back(s_CONTENT_INTERFACE);
    m_AboutService->AddObjectDescription(s_CONTENT_OBJECT_PATH, mdfInterfaces);
    //std::vector<qcc::String> const& interfaceNames

    QCC_DbgTrace(("m_BusAttachment->RegisterBusObject: AboutService"));
    status = m_BusAttachment->RegisterBusObject(*m_AboutService);
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to register the about service"));
        return status;
    }

    QCC_DbgTrace(("m_BusAttachment->RegisterBusObject: ContentServerBusObject"));
    status = m_BusAttachment->RegisterBusObject(*m_ContentServerBusObject);
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to register m_ContentServerBusObject"));
        return status;
    }

    return ER_OK;
}

QStatus SampleContentService::InitContentModel()
{
    QCC_DbgTrace(("InitContentModel"));

    QCC_DbgTrace(("CreateBusAttachment"));
    m_BusAttachment = new BusAttachment("MDF SampleContent Server", true); //Make name unique
    if (m_BusAttachment == NULL) {
        QCC_LogError(ER_OUT_OF_MEMORY, ("Create Bus attachment failed"));
        return ER_OUT_OF_MEMORY;
    }

    QCC_DbgTrace(("ContentBusListener"));
    m_ContentBusListener = new ContentBusListener();
    if (m_ContentBusListener == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("could not initialise the bus listener"));
        return ER_INIT_FAILED;
    }

    QCC_DbgTrace(("m_BusAttachment->RegisterBusListener"));
    m_BusAttachment->RegisterBusListener(*m_ContentBusListener);
    QStatus status = m_BusAttachment->Start();

    QCC_DbgTrace(("SampleContentSource"));
    m_MdfContentSource = new SampleContentSource();
    if (m_MdfContentSource == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("Create contentSource failed"));
        return status;
    }

    QCC_DbgTrace(("Init - Server m_BusAttachment busName=%s", m_BusAttachment->GetUniqueName().c_str()));

    QCC_DbgTrace(("ContentServerBusObject"));
    m_ContentServerBusObject = new ContentServerBusObject(s_CONTENT_OBJECT_PATH, m_MdfContentSource);

    status = m_ContentServerBusObject->CreateInterface(*m_BusAttachment);
    if (status != ER_OK) {
        QCC_LogError(status, ("m_ContentServerBusObject->CreateInterface failed"));
        return status;
    }

    return ER_OK;
}

QStatus SampleContentService::InitContentComms()
{
    QCC_DbgTrace(("InitContentComms"));
    m_SessionPort = 1288;

    QCC_DbgTrace(("m_BusAttachment->Connect"));
    QStatus status = m_BusAttachment->Connect();
    if (status != ER_OK) {
        QCC_LogError(status, ("m_BusAttachment->Connect"));
        return status;
    }

    QCC_DbgTrace(("SessionOpts"));
    m_sessionOpts = new SessionOpts(SessionOpts::TRAFFIC_MESSAGES, false, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);
    if (m_sessionOpts == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("could not initialise the session opts"));
        return ER_INIT_FAILED;
    }

    QCC_DbgTrace(("ContentSessionPortListener"));
    m_ContentSessionPortListener = new ContentSessionPortListener();

    QCC_DbgTrace(("m_BusAttachment->BindSessionPort"));
    status = m_BusAttachment->BindSessionPort(m_SessionPort, *m_sessionOpts, *m_ContentSessionPortListener);
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to bind to the session port"));
        return status;
    }

    //now we create the interface
    QCC_DbgTrace(("InitAboutService"));
    status = InitAboutService(m_BusAttachment, m_SessionPort);
    if (status != ER_OK) {
        return status;
    }

    //This announces the model (methods etc). Can re-announce after change in meta-data, is possible - For example for DEVICE_NAME (Config will use this automatically)
    //Reconnect to the routing node -> also re-announce if busUniqueName changes.
    QCC_DbgTrace(("m_AboutService->Announce"));
    status = m_AboutService->Announce(); //CB is on the BindSessionPort(portListner)->AcceptSessionJoiner

    if (status != ER_OK) {
        QCC_LogError(status, ("Failed m_AboutService->Announce"));
        return status;
    }

    return ER_OK;
}

QStatus SampleContentService::Init()
{
    QStatus status = InitContentModel();
    if (status == ER_OK) {
        InitContentComms();
    }
    return status;
}

AboutPropertyStoreImpl* SampleContentService::CreateAboutPropertyStore()
{
    QCC_DbgTrace(("SampleContentService::CreateAboutPropertyStore"));
    AboutPropertyStoreImpl* propertyStore = new AboutPropertyStoreImpl();

    if (propertyStore == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("Could not initialise the property store"));
        return NULL;
    }

    propertyStore->setDeviceId("ContentDevice-1");
    propertyStore->setDeviceName("SampleContentServer", "en");
    propertyStore->setAppId("abcd1234efghij56789");
    propertyStore->setAppName("MDF Sample Content Server");

    std::vector<qcc::String> languages(1);
    languages[0] = "en";
    propertyStore->setSupportedLangs(languages);
    propertyStore->setDefaultLang(languages[0]);

    propertyStore->setModelNumber("Wxfy388i");
    propertyStore->setDateOfManufacture("12/10/2015");
    propertyStore->setSoftwareVersion("0.1 build 1");
    propertyStore->setAjSoftwareVersion(ajn::GetVersion());
    propertyStore->setHardwareVersion("34523d");

    propertyStore->setDescription("Allseen sample media content application", "en");
    propertyStore->setManufacturer("Allseen Alliance", "en");

    propertyStore->setSupportUrl("http://www.allseen.org");

    return propertyStore;
}

} /* namespace mdf */
