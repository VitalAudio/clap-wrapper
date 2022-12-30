#pragma once

#include <clap/clap.h>

// ARA support
// 

static CLAP_CONSTEXPR const char CLAP_EXT_ARA_MAINFACTORY[] = "com.celemony.ara.mainfactory/draft.0";
static CLAP_CONSTEXPR const char CLAP_EXT_ARA_IPLUGINENTRYPOINT2[] = "com.celemony.ara.pluginentrypoint2/draft.0";

#ifdef __cplusplus
extern "C" {
#endif

  // substitute types so we don't need to include the ARA SDK in full
  typedef void* ARAFactoryPtr;
  typedef void* ARAPlugInExtensionInstancePtr;
  typedef void* ARADocumentControllerRef;
  typedef int32_t ARAPlugInInstanceRoleFlags;

  typedef struct clap_plugin_ara_mainfactory
  {
    //! Get the ARA factory.
    //! The returned pointer must remain valid throughout the lifetime of the object that provided it.
    //! The returned ARAFactory must be equal to the ARAFactory provided by the associated
    //! plugin object through its clap_plugin_ara_pluginentrypoint extension.
    const void*(CLAP_ABI* getFactory)();
  } clap_plugin_ara_mainfactory_t;

  typedef struct clap_plugin_ara_pluginentrypoint
  {
    const ARAPlugInExtensionInstancePtr(CLAP_ABI* bindToDocumentControllerWithRoles)(const clap_plugin_t* plugin, ARADocumentControllerRef documentControllerRef,
      ARAPlugInInstanceRoleFlags knownRoles, ARAPlugInInstanceRoleFlags assignedRoles) = 0;
  } clap_plugin_ara_pluginentrypoint_t;


#ifdef __cplusplus
}
#endif
