<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * Gallery core module.
 * @package GalleryCore
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 20994 $
 */
class CoreModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('core');
		$this->setName($gallery->i18n('Core'));
		$this->setDescription(
			$gallery->i18n(
				'Gallery 2 Core Module'
			)
		);

		// Update CoreModuleExtras::upgrade and CoreModuleExtras::_prepareConfigUpgrade too!
		$this->setVersion('1.4.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;
		$this->setGalleryVersion('2.4.0');

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks(
			'getItemLinks|getSystemLinks|getItemAdminViews|getSiteAdminViews|getUserAdminViews'
		);
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new CoreModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::getItemLinks
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions, $userId = null) {
		global $gallery;
		$urlGenerator =& $gallery->getUrlGenerator();

		list($ret, $rootId) = GalleryCoreApi::getPluginParameter(
			'module',
			'core',
			'id.rootAlbum'
		);

		if ($ret) {
			return array($ret, null);
		}

		// Fetch child counts
		$itemIds = array();

		foreach ($items as $item) {
			$itemIds[] = $item->getId();
		}

		/*
		 * We're not doing this as the acting user id, but that's OK because we're not displaying
		 * the result; we're only using it as a gating factor for whether or not we show a link, and
		 * that'll also be gated by the permissions we receive which *will* be for the acting user.
		 */
		list($ret, $childCounts) = GalleryCoreApi::fetchChildCounts($itemIds);

		if ($ret) {
			return array($ret, null);
		}

		// Fetch thumbnail ids
		list($ret, $thumbTable) = GalleryCoreApi::fetchThumbnailsByItemIds($itemIds);

		if ($ret) {
			return array($ret, null);
		}

		$links = array();

		foreach ($items as $item) {
			$itemId  = $item->getId();
			$isRoot  = ($itemId == $rootId);
			$isAlbum = $item->getCanContainChildren();

			// Permissions for its parent
			list($ret, $parentPermissions) = GalleryCoreApi::getPermissions(
				$item->getParentId(),
				$userId
			);

			if ($ret) {
				return array($ret, null);
			}

			if (isset($wantsDetailedLinks[$itemId]) && $isAlbum
				&& isset($permissions[$itemId]['core.addDataItem'])
			) {
				$links[$itemId][] = array(
					'text'   => $this->_translate('Add Items'),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemAdd',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}

			$itemTypeNames = array_merge($item->itemTypeName(), $item->itemTypeName(false));

			if (isset($permissions[$itemId]['core.edit'])) {
				if (false) {
					// Specific translations:
					_('Edit Album');
					_('Edit Photo');
					_('Edit Movie');
				}
				$links[$itemId][] = array(
					'text' => $this->_translate(
						array(
							'text' => 'Edit %s',
							'arg1' => $itemTypeNames[0],
						),
						$itemTypeNames[2]
					),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemEdit',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}

			if (isset($wantsDetailedLinks[$itemId]) && $isAlbum
				&& isset($permissions[$itemId]['core.addAlbumItem'])
			) {
				$links[$itemId][] = array(
					'text'   => $this->_translate('Add Album'),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemAddAlbum',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}

			if (isset($permissions[$itemId]['core.edit'])) {
				if (isset($permissions[$itemId]['core.changePermissions'])) {
					$itemPerm = $this->_translate('Edit Permissions');
				} else {
					$itemPerm = $this->_translate('View Permissions');
				}

				$links[$itemId][] = array(
					'text'   => $itemPerm,
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemPermissions',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}

			if (!$isRoot && isset($permissions[$itemId]['core.delete'])) {
				if (false) {
					// Specific translations:
					_('Delete Album');
					_('Delete Photo');
					_('Delete Movie');
				}

				if (!function_exists('smarty_modifier_escape')) {
					GalleryCoreApi::requireOnce('lib/smarty/plugins/modifier.escape.php');
				}
				$title = smarty_modifier_escape(
					GalleryUtilities::markup(
						$item->getTitle(),
						'strip'
					),
					'javascript',
					'UTF-8'
				);

				list($thisItemId, $page) = GalleryUtilities::getRequestVariables('itemId', 'page');
				// If no page item defined, we will navigate to the parent of the deleted item
				if (empty($thisItemId)) {
					$thisItemId = $itemId;
				}

				$urlParams = array(
					'controller' => 'core.ItemDeleteSingle',
					'itemId'     => $itemId,
					'pageId'     => $thisItemId,
				);

				if (!empty($page)) {
					$urlParams['page'] = $page;
				}

				$url = $urlGenerator->generateUrl($urlParams);

				if ($isAlbum) {
					$moreUrl = '';
				} else {
					$moreUrl = $urlGenerator->generateUrl(
						array(
							'view'       => 'core.ItemAdmin',
							'subView'    => 'core.ItemDelete',
							'itemId'     => $item->getParentId(),
							'selectedId' => $itemId,
							'return'     => 1,
						)
					);
				}

				$links[$itemId][] = array(
					'text'   => $this->_translate(
						array(
							'text' => 'Delete %s',
							'arg1' => $itemTypeNames[0],
						),
						$itemTypeNames[2]
					),
					// Params are for g->linkId used by icons, or themes ignoring 'script'
					'params' => array(
						'view'       => 'core.ItemAdmin',
						'subView'    => 'core.ItemDelete',
						'itemId'     => $item->getParentId(),
						'selectedId' => $itemId,
					),
					'script' => "core_confirmDelete('$url', '$moreUrl', '$title')",
				);
			}

			if (!$isRoot && isset($permissions[$itemId]['core.delete'])) {
				if (false) {
					// Specific translations:
					_('Move Album');
					_('Move Photo');
					_('Move Movie');
				}
				$links[$itemId][] = array(
					'text' => $this->_translate(
						array(
							'text' => 'Move %s',
							'arg1' => $itemTypeNames[0],
						),
						$itemTypeNames[2]
					),
					'params' => array(
						'view'       => 'core.ItemAdmin',
						'subView'    => 'core.ItemMove',
						'itemId'     => $item->getParentId(),
						'selectedId' => $itemId,
						'return'     => 1,
					),
				);
			}

			if (isset($wantsDetailedLinks[$itemId])
				&& $isAlbum
				&& isset($permissions[$itemId]['core.edit'])
				&& !empty($childCounts[$itemId])
				&& $childCounts[$itemId] > 0
			) {
				$link = array(
					'text'   => $this->_translate('Edit Captions'),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemEditCaptions',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);

				list($thisItemId, $thisPage) = GalleryUtilities::getRequestVariables(
					'itemId',
					'page'
				);

				if (!empty($thisItemId) && !empty($thisPage) && $thisItemId == $item->getId()) {
					$link['params']['albumPage'] = $thisPage;
				}
				$links[$itemId][] = $link;
			}

			if (!$isRoot && isset($thumbTable[$itemId], $parentPermissions['core.edit'])) {
				$links[$itemId][] = array(
					'text'   => $this->_translate('Make Highlight'),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemMakeHighlight',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}

			if ($isAlbum && isset($permissions[$itemId]['core.edit'])
				&& !empty($childCounts[$itemId]) && $childCounts[$itemId] > 1
			) {
				$links[$itemId][] = array(
					'text'   => $this->_translate('Reorder Items'),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemReorder',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}

			if (isset($wantsDetailedLinks[$itemId])
				&& $isAlbum
				&& isset($permissions[$itemId]['core.edit'])
				&& !empty($childCounts[$itemId])
				&& $childCounts[$itemId] > 0
			) {
				$next   = next($items);
				$params = array(
					'view'    => 'core.ItemAdmin',
					'subView' => 'core.ItemDelete',
					'itemId'  => $itemId,
					'return'  => 1,
				);
				// First item on the page will be in the list for deletion
				if ($next) {
					$params['anchorId'] = $next->getId();
				}

				$links[$itemId][] = array(
					'text'   => $this->_translate('Delete Items'),
					'params' => $params,
				);
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getSystemLinks
	 */
	public function getSystemLinks() {
		global $gallery;

		list($ret, $param) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

		if ($ret) {
			return array($ret, null);
		}

		$links               = array();
		list($ret, $isAdmin) = GalleryCoreApi::isUserInSiteAdminGroup();

		if ($ret) {
			return array($ret, null);
		}

		if ($isAdmin) {
			$links['SiteAdmin'] = array(
				'text'   => $this->translate('Site Admin'),
				'params' => array(
					'view'   => 'core.SiteAdmin',
					'return' => 1,
				),
			);
		}

		if ($gallery->getConfig('login')) {
			list($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser();

			if ($ret) {
				return array($ret, null);
			}

			if ($isAnonymous) {
				$links['Login'] = array(
					'text'   => $this->translate('Login'),
					'params' => array(
						'view'    => 'core.UserAdmin',
						'subView' => 'core.UserLogin',
						'return'  => 1,
					),
				);
			} else {
				$user                 = $gallery->getActiveUser();
				$links['YourAccount'] = array(
					'text'   => $this->translate('Your Account'),
					'params' => array(
						'view'    => 'core.UserAdmin',
						'subView' => 'core.UserPreferences',
						'return'  => 1,
					),
				);
				$links['Logout']      = array(
					'text'   => $this->translate('Logout'),
					'params' => array(
						'controller' => 'core.Logout',
						'return'     => 1,
					),
				);
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		$data = array(
			array(
				'name' => $this->translate('General'),
				'view' => 'core.AdminCore',
			),
			array(
				'name' => $this->translate('Plugins'),
				'view' => 'core.AdminPlugins',
			),
			array(
				'name' => $this->translate('Users'),
				'view' => 'core.AdminUsers',
			),
			array(
				'name' => $this->translate('Groups'),
				'view' => 'core.AdminGroups',
			),
			array(
				'name' => $this->translate('Maintenance'),
				'view' => 'core.AdminMaintenance',
			),
			array(
				'name' => $this->translate('Themes'),
				'view' => 'core.AdminThemes',
			),
			array(
				'name' => $this->translate('Performance'),
				'view' => 'core.AdminPerformance',
			),
			array(
				'name' => $this->translate('Event Logs'),
				'view' => 'core.AdminEventLogViewer',
			),
			array(
				'name' => $this->translate('Language Settings'),
				'view' => 'core.AdminLanguageManager',
			),
		);

		list($ret, $list) = GalleryCoreApi::getRedundantToolkitPriorities();

		if ($ret) {
			return array($ret, null);
		}

		if (!empty($list)) {
			$data[] = array(
				'name'       => $this->translate('Toolkit Priority'),
				'view'       => 'core.AdminToolkitPriority',
				'group'      => 'toolkits',
				'groupLabel' => $this->translate('Graphics Toolkits'),
			);
		}

		return array(null, $data);
	}

	/**
	 * @see GalleryModule::getUserAdminViews
	 */
	public function getUserAdminViews($user) {
		global $gallery;
		$views = array();

		if ($gallery->getConfig('login')) {
			list($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser($user->getId());

			if ($ret) {
				return array($ret, null);
			}

			if (!$isAnonymous) {
				if (!$user->isLocked()) {
					$views[] = array(
						'name' => $this->translate('Account Settings'),
						'view' => 'core.UserPreferences',
					);
					$views[] = array(
						'name' => $this->translate('Change Password'),
						'view' => 'core.UserChangePassword',
					);
				}
			} else {
				$views[] = array(
					'name' => $this->translate('Login'),
					'view' => 'core.UserLogin',
				);
			}
		}

		return array(null, $views);
	}

	/**
	 * @see GalleryModule::getItemAdminViews
	 */
	public function getItemAdminViews($item) {
		global $gallery;

		$views                   = array();
		list($ret, $permissions) = GalleryCoreApi::getPermissions($item->getId());

		if ($ret) {
			return array($ret, null);
		}

		$childCount = 0;
		$isAlbum    = $item->getCanContainChildren();

		if ($isAlbum) {
			list($ret, $childCounts) = GalleryCoreApi::fetchChildCounts(array($item->getId()));

			if ($ret) {
				return array($ret, null);
			}

			if (empty($childCounts[$item->getId()])) {
				$childCount = 0;
			} else {
				$childCount = $childCounts[$item->getId()];
			}
		}

		// Fetch thumbnail ids
		list($ret, $thumbTable) = GalleryCoreApi::fetchThumbnailsByItemIds(array($item->getId()));

		if ($ret) {
			return array($ret, null);
		}
		$hasThumb = !empty($thumbTable[$item->getId()]);

		$parentId                      = $item->getParentId();
		list($ret, $parentPermissions) = GalleryCoreApi::getPermissions($parentId);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $rootId) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.rootAlbum');

		if ($ret) {
			return array($ret, null);
		}

		$itemTypeNames = array_merge($item->itemTypeName(), $item->itemTypeName(false));

		if (isset($permissions['core.edit'])) {
			if ($isAlbum && $childCount > 1) {
				$views[] = array(
					'name' => $this->_translate('Reorder Items'),
					'view' => 'core.ItemReorder',
				);
			}

			if ($isAlbum && $childCount > 0) {
				$views[] = array(
					'name' => $this->_translate('Edit Captions'),
					'view' => 'core.ItemEditCaptions',
				);
			}

			// Edit view for all item types
			$views[] = array(
				'name' => $this->_translate(
					array(
						'text' => 'Edit %s',
						'arg1' => $itemTypeNames[0],
					),
					$itemTypeNames[2]
				),
				'view' => 'core.ItemEdit',
			);

			if (isset($permissions['core.changePermissions'])) {
				$viewsPerm = $this->_translate('Edit Permissions');
			} else {
				$viewsPerm = $this->_translate('View Permissions');
			}

			$views[] = array(
				'name' => $viewsPerm,
				'view' => 'core.ItemPermissions',
			);
		}

		if (!empty($parentId) && $hasThumb && isset($parentPermissions['core.edit'])) {
			$views[] = array(
				'name' => $this->_translate('Make Highlight'),
				'view' => 'core.ItemMakeHighlight',
			);
		}

		if (isset($permissions['core.delete'])) {
			$itemId = $item->getId();

			if ($itemId != $rootId) {
				if (!function_exists('smarty_modifier_escape')) {
					GalleryCoreApi::requireOnce('lib/smarty/plugins/modifier.escape.php');
				}
				$title = smarty_modifier_escape(
					GalleryUtilities::markup(
						$item->getTitle(),
						'strip'
					),
					'javascript',
					'UTF-8'
				);
				// This is ItemAdmin view, hence page item is the one we are deleting
				$thisItemId   = $itemId;
				$urlGenerator =& $gallery->getUrlGenerator();
				$url          = $urlGenerator->generateUrl(
					array(
						'controller' => 'core.ItemDeleteSingle',
						'itemId'     => $itemId,
						'pageId'     => $thisItemId,
					)
				);

				if ($isAlbum) {
					$moreUrl = '';
				} else {
					$moreUrl = $urlGenerator->generateUrl(
						array(
							'view'       => 'core.ItemAdmin',
							'subView'    => 'core.ItemDelete',
							'itemId'     => $item->getParentId(),
							'selectedId' => $itemId,
						)
					);
				}

				$views[] = array(
					'name'   => $this->_translate(
						array(
							'text' => 'Delete %s',
							'arg1' => $itemTypeNames[0],
						),
						$itemTypeNames[2]
					),
					'view'   => 'core.ItemDeleteSingle', // just for g->linkId used by icons
					'script' => "core_confirmDelete('$url', '$moreUrl', '$title');return false",
				);

				$views[] = array(
					'name' => $this->_translate(
						array(
							'text' => 'Move %s',
							'arg1' => $itemTypeNames[0],
						),
						$itemTypeNames[2]
					),
					'view' => 'core.ItemMoveSingle',
				);
			}

			if ($isAlbum && $childCount > 0) {
				$views[] = array(
					'name' => $this->_translate('Delete Items'),
					'view' => 'core.ItemDelete',
				);
			}
		}

		if (isset($permissions['core.addDataItem']) && $isAlbum) {
			$views[] = array(
				'name' => $this->_translate('Add Items'),
				'view' => 'core.ItemAdd',
			);
		}

		if (isset($permissions['core.addAlbumItem']) && $isAlbum) {
			$views[] = array(
				'name' => $this->_translate('Add Album'),
				'view' => 'core.ItemAddAlbum',
			);
		}

		return array(null, $views);
	}

	/**
	 * We pushed all this code into its own file since we need it very rarely so it doesn't make
	 * sense to load it every single time.
	 *
	 * @see GalleryModule::install
	 */
	public function upgrade($currentVersion, $statusMonitor = null) {
		GalleryCoreApi::requireOnce('modules/core/CoreModuleExtras.inc');
		$ret = CoreModuleExtras::getMe()->upgrade($this, $currentVersion, $statusMonitor);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * Set the current version of Gallery.
	 */
	public function setGalleryVersion($version) {
		$this->_galleryVersion = $version;
	}

	/**
	 * Get the current version of Gallery.
	 */
	public function getGalleryVersion() {
		return $this->_galleryVersion;
	}

	/**
	 * Get the version of the core module and of Gallery itself.  We store this on disk to avoid
	 * having to load up the database (which can be problematic if we're doing an upgrade and don't
	 * want to count a specific database schema). If the versions.dat file indicates that Gallery
	 * is in maintenance mode, then set the configuration value to indicate this.
	 *
	 * @return array 'core' => core module version, 'gallery' => gallery version
	 * @static
	 */
	public function getInstalledVersions() {
		global $gallery;

		if (GalleryDataCache::containsKey('CoreModule::installedVersions')) {
			$versions = GalleryDataCache::get('CoreModule::installedVersions');
		} else {
			$platform    =& $gallery->getPlatform();
			$versionFile = $gallery->getConfig('data.gallery.version');

			if ($platform->file_exists($versionFile)) {
				$versionArray = $platform->file($versionFile);

				if (count($versionArray) >= 2) {
					$versions['core']    = rtrim($versionArray[0]);
					$versions['gallery'] = rtrim($versionArray[1]);
					GalleryDataCache::put('CoreModule::installedVersions', $versions, true);

					if (isset($versionArray[2])) {
						$maintenanceMode = explode(':', rtrim($versionArray[2]), 2);

						if ($maintenanceMode[0] == 'maintenance-mode') {
							if (empty($maintenanceMode[1])) {
								$newMode = true;
							} else {
								$newMode = $maintenanceMode[1];
							}

							$gallery->setConfig('mode.maintenance', $newMode);
						}
					}
				}
			} else {
				/*
				 * If we get here we are probably doing a fresh install so just return 0, 0 and
				 * the rest of the install should work properly.
				 */
				$versions = array(
					'core'    => 0,
					'gallery' => 0,
				);
			}
		}

		return $versions;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		GalleryCoreApi::requireOnce('modules/core/CoreModuleExtras.inc');
		$ret = CoreModuleExtras::getMe()->performFactoryRegistrations($this);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
