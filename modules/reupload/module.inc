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
 * Reupload a file without destroying metadata information
 *
 * @package Reupload
 * @author Piotr P. Karwasz <piotr.karwasz@ens.fr>
 * @version $Revision: 18172 $
 */
class ReuploadModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('reupload');
		$this->setName($gallery->i18n('Reupload'));
		$this->setDescription(
			$gallery->i18n(
				'Allows reuploading Gallery Items'
			)
		);

		$this->setVersion('2.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('data', $gallery->i18n('Extra Data'));
		$this->setCallbacks('');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new ReuploadModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditOption',
			'ReuploadPhotoOption',
			'ReuploadPhotoOption',
			'modules/reupload/ReuploadPhotoOption.inc',
			'reupload',
			array('ItemEditRotateAndScalePhoto')
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
