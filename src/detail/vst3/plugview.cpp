#include "plugview.h"
#include <clap/clap.h>
#include <crtdbg.h>
#include <cassert>

WrappedView::WrappedView(const clap_plugin_t* plugin, const clap_plugin_gui_t* gui)
  : FObject()
  , IPlugView()
  , _plugin(plugin)
  , _extgui(gui)
{
  
}

WrappedView::~WrappedView()
{
  drop_ui();
}

void WrappedView::ensure_ui()
{
  if (!_created)
  {
    _extgui->create(_plugin, CLAP_WINDOW_API_WIN32, false);
    _created = true;
  }
}

void WrappedView::drop_ui()
{
  if (_created)
  {
    _created = false;
    _attached = false;
    _extgui->destroy(_plugin);
  }
}

tresult PLUGIN_API WrappedView::isPlatformTypeSupported(FIDString type)
{
  static struct vst3_and_clap_match_types_t{
    const char* VST3;
    const char* CLAP;
  } platformTypeMatches[] =
    {
      { kPlatformTypeHWND, CLAP_WINDOW_API_WIN32},
      { kPlatformTypeNSView, CLAP_WINDOW_API_COCOA },
      { kPlatformTypeX11EmbedWindowID, CLAP_WINDOW_API_X11},
      { nullptr, nullptr }
    };
  auto* n = platformTypeMatches;
  while (n->VST3 && n->CLAP)
  {
    if (!strcmp(type, n->VST3))
    {
      if (_extgui->is_api_supported(_plugin, n->CLAP, false))
      {
        return kResultOk;
      }
    }
    ++n;
  }

  return kResultFalse;
}

tresult PLUGIN_API WrappedView::attached(void* parent, FIDString type)
{  
  _window = { CLAP_WINDOW_API_WIN32, parent };

  ensure_ui();
  _extgui->set_parent(_plugin, &_window);
  _attached = true;
  if (_extgui->can_resize(_plugin))
  {
    _extgui->set_size(_plugin, _rect.getWidth(), _rect.getHeight());
  }
  return kResultOk;
}

tresult PLUGIN_API WrappedView::removed()
{
  drop_ui();
  _window.ptr = nullptr;
  return kResultOk;
}

tresult PLUGIN_API WrappedView::onWheel(float distance)
{
  return kResultOk;
}

tresult PLUGIN_API WrappedView::onKeyDown(char16 key, int16 keyCode, int16 modifiers)
{
  return kResultOk;
}

tresult PLUGIN_API WrappedView::onKeyUp(char16 key, int16 keyCode, int16 modifiers)
{
  return kResultOk;
}

tresult PLUGIN_API WrappedView::getSize(ViewRect* size)
{
  ensure_ui();
  if (size)
  {
    uint32_t w, h;
    if (_extgui->get_size(_plugin, &w, &h))
    {
      size->right = size->left + w;
      size->bottom = size->top + h;
      _rect = *size;
      return kResultOk;
    }
    return kResultFalse;
  }
  return kInvalidArgument;
}

tresult PLUGIN_API WrappedView::onSize(ViewRect* newSize)
{
  // TODO: discussion took place if this call should be ignored completely
  // since it seems not to match the CLAP UI scheme.
  // for now, it is left in and might be removed in the future.
  if (newSize)
  {
    _rect = *newSize;
    if (_created && _attached)
    {
      if (_extgui->can_resize(_plugin))
      {
        if (_extgui->set_size(_plugin, newSize->getWidth(), newSize->getHeight()))
        {
          return kResultOk;
        }
      }
      else
      {
        return kResultFalse;
      }

    }
    return kResultOk;
  }
  
  return kResultFalse;
}

tresult PLUGIN_API WrappedView::onFocus(TBool state)
{
  // TODO: this might be something for the wrapperhost API
  // to notify the plugin about a focus change
  return kResultOk;
}

tresult PLUGIN_API WrappedView::setFrame(IPlugFrame* frame)
{
  _plugFrame = frame;
  return kResultOk;
}

tresult PLUGIN_API WrappedView::canResize()
{
  ensure_ui();
  return _extgui->can_resize(_plugin) ? kResultOk : kResultFalse;
}

tresult PLUGIN_API WrappedView::checkSizeConstraint(ViewRect* rect)
{
  ensure_ui();
  uint32_t w = rect->getWidth();
  uint32_t h = rect->getHeight();
  if (_extgui->adjust_size(_plugin, &w, &h))
  {
    rect->right = rect->left + w;
    rect->bottom = rect->top + h;
    return kResultOk;
  }
  return kResultFalse;
}