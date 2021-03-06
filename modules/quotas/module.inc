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
 * The implementation of the core-module
 *
 * @package Quotas
 * @author Robert Balousek <volksport@users.sf.net>
 * @version $Revision: 18172 $
 */
class QuotasModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('quotas');
		$this->setName($gallery->i18n('Quotas'));
		$this->setDescription(
			$gallery->i18n(
				'User/Group Disk Quotas'
			)
		);

		$this->setVersion('2.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new QuotasModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion, $x1 = null) {
		global $gallery;

		if (!empty($currentVersion) && version_compare($currentVersion, '1.0.1', '<')) {
			$storage =& $gallery->getStorage();
			$ret     = $storage->configureStore($this->getId(), array('QuotasMap:1.0'));

			if ($ret) {
				return $ret;
			}
		}

		if (!empty($currentVersion) && version_compare($currentVersion, '1.0.7', '<')) {
			// Cleanup any factory data from way back in v0.9.3 that had wrong module id
			$ret = GalleryCoreApi::unregisterFactoryImplementationsByModuleId('diskquota');

			if ($ret) {
				return $ret;
			}
		}

		return null;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Quotas'),
					'view' => 'quotas.AdminQuotas',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'QuotasModule',
			'QuotasModule',
			'modules/quotas/module.inc',
			'quotas',
			array('GalleryEntity::delete')
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemAddOption',
			'DiskQuotaOption',
			'DiskQuotaOption',
			'modules/quotas/DiskQuotaOption.inc',
			'quotas',
			null,
			9
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * Event handler for GalleryEntity::delete events
	 * Delete any group/user quotas if the associated group/user is deleted
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		$result = null;

		if ($event->getEventName() == 'GalleryEntity::delete') {
			$entity = $event->getEntity();

			if (GalleryUtilities::isA($entity, 'GalleryGroup')
				|| GalleryUtilities::isA($entity, 'GalleryUser')
			) {
				GalleryCoreApi::requireOnce(
					'modules/quotas/classes/GalleryQuotasHelper.class'
				);

				if (GalleryUtilities::isA($entity, 'GalleryGroup')) {
					// Delete group quota if exists and entity is a group
					list($ret, $quotaExists, $groupQuota) = GalleryQuotasHelper::fetchGroupQuota(
						$entity->getId()
					);

					if ($ret) {
						return array($ret, null);
					}
				} elseif (GalleryUtilities::isA($entity, 'GalleryUser')) {
					// Delete user quota if exists and entity is a user
					list($ret, $quotaExists, $userQuota) = GalleryQuotasHelper::fetchUserQuota(
						$entity->getId()
					);

					if ($ret) {
						return array($ret, null);
					}
				}

				if ($quotaExists) {
					$ret = GalleryCoreApi::removeMapEntry(
						'QuotasMap',
						array(
							'userOrGroupId' => $entity->getId(),
						)
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}
		}

		return array(null, $result);
	}
}
