/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_PLAYLIST_PLAYLIST_DOWNLOAD_REQUEST_MANAGER_H_
#define BRAVE_COMPONENTS_PLAYLIST_PLAYLIST_DOWNLOAD_REQUEST_MANAGER_H_

#include <list>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "brave/components/playlist/media_detector_component_manager.h"
#include "brave/components/playlist/playlist_types.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"

namespace base {
class OneShotTimer;
class Value;
}  // namespace base

namespace content {
class BrowserContext;
}  // namespace content

namespace playlist {

// This class finds media files and their thumbnails and title from a page
// by injecting media detector script to dedicated WebContents.
class PlaylistDownloadRequestManager
    : public MediaDetectorComponentManager::Observer,
      public content::WebContentsObserver {
 public:
  class Delegate {
   public:
    virtual void OnPlaylistCreationParamsReady(
        const PlaylistItemInfo& params) = 0;
  };

  static void SetPlaylistJavaScriptWorldId(const int32_t id);

  PlaylistDownloadRequestManager(content::BrowserContext* context,
                                 Delegate* delegate,
                                 MediaDetectorComponentManager* manager);
  ~PlaylistDownloadRequestManager() override;
  PlaylistDownloadRequestManager(const PlaylistDownloadRequestManager&) =
      delete;
  PlaylistDownloadRequestManager& operator=(
      const PlaylistDownloadRequestManager&) = delete;

  // Delegate will get called with generated param.
  void GetMediaFilesFromPage(const std::string& url);

 private:
  // Calling this will trigger loading |url| on a web contents,
  // and we'll inject javascript on the contents to get a list of
  // media files on the page.
  void RunMediaDetector(const std::string& url);

  bool ReadyToRunMediaDetectorScript() const;
  void CreateWebContents();
  void OnGetMedia(base::Value value);

  // Pop a task from queue and detect media from the page if any.
  void FetchPendingURL();

  void ScheduleWebContentsDestroying();
  void DestroyWebContents();

  // content::WebContentsObserver overrides:
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;
  void DidFinishLoad(content::RenderFrameHost* render_frame_host,
                     const GURL& validated_url) override;

  // MediaDetectorComponentManager::Observer overrides:
  void OnScriptReady(const std::string& script) override;

  // We create |web_contents_| on demand. So, when downloading media is
  // requested, |web_contents_| may not be ready to inject js script. This
  // list caches already requested urls and used after |web_contents_| is
  // ready to use.
  std::list<std::string> pending_urls_;

  // Used to inject js script to get playlist item metadata to download
  // its media files/thumbnail images and get titile.
  std::unique_ptr<content::WebContents> web_contents_;

  // The number of requested data fetching.
  // If it's zero, all requested fetching are completed. Then |web_contents_|
  // destroying task will be scheduled.
  int in_progress_urls_count_ = 0;

  std::string media_detector_script_;
  raw_ptr<content::BrowserContext> context_;
  raw_ptr<Delegate> delegate_;

  raw_ptr<MediaDetectorComponentManager> media_detector_component_manager_;
  base::ScopedObservation<MediaDetectorComponentManager,
                          MediaDetectorComponentManager::Observer>
      observed_{this};
  std::unique_ptr<base::OneShotTimer> web_contents_destroy_timer_;

  base::WeakPtrFactory<PlaylistDownloadRequestManager> weak_factory_{this};
};

}  // namespace playlist

#endif  // BRAVE_COMPONENTS_PLAYLIST_PLAYLIST_DOWNLOAD_REQUEST_MANAGER_H_
