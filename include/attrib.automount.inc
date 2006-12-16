<?php
// attribute plugin for
// automount maps
// $Id: attrib.automount.inc,v 2.4 2006-12-16 12:02:14 turbo Exp $

// {{{ Create the automountInformation attribute
function attribute_setup() {
  $automount = "-fstype=".$_REQUEST["filesystem"];
  if(@$_REQUEST["options"])
	$automount .= ",".$_REQUEST["options"];

  // FS: SMB
  if($_REQUEST["filesystem"] == 'smbfs') {
	// SMB mount options
	if($_REQUEST["smbcredtype"] == 'guest') {
	  $automount .= ",guest";
	} elseif($_REQUEST["smbcredtype"] == 'file') {
	  $automount .= ",credentials=".$_REQUEST["smbcredfile"];
	} else {
	  if(@$_REQUEST["smbcreduser"])
		$automount .= ",username=".$_REQUEST["smbcreduser"];
	  if(@$_REQUEST["smbcredpasswd"])
		$automount .= ",password=".$_REQUEST["smbcredpasswd"];
	}
	if(@$_REQUEST["smbcreduid"])
	  $automount .= ",uid=".$_REQUEST["smbcreduid"];
	if(@$_REQUEST["smbcredgid"])
	  $automount .= ",gid=".$_REQUEST["smbcredgid"];
	
	// SMB mount
	$automount .= " ://".$_REQUEST["smbserver"];
	if(!ereg('^/', $_REQUEST["smbshare"]))
	  $automount .= "/";
	$automount .= $_REQUEST["smbshare"];

  // FS: NFS/NFS4
  } elseif(($_REQUEST["filesystem"] == 'nfs') or ($_REQUEST["filesystem"] == 'nfs4')) {
	$automount .= " ".$_REQUEST["nfsserver"];
	if(!ereg('^/', $_REQUEST["nfsshare"]))
	  $automount .= "/";
	$automount .= ":".$_REQUEST["nfsshare"];

  // FS: NCP
  } elseif($_REQUEST["filesystem"] == 'ncpfs') {
	$automount .= " ".$_REQUEST["ncpserver"];
	if(!ereg('^/', $_REQUEST["ncpshare"]))
	  $automount .= "/";
	$automount .= ":".$_REQUEST["ncpshare"];


  // FS: Other
  } else {
	// 'normal' mountpoint
	$automount .= " :";
	$automount .= $_REQUEST["device"];
  }

  return $automount;
}
// }}}

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;
    
	if($_REQUEST["dosave"] == 'yes') {
	  // We've clicked the Save button
	  if($_REQUEST["basemount"] == 'new') {
		// Create a new base mountpoint
		if(!$_REQUEST["mountpoint"]) {
		  $error["mountpoint"] = $LANG->_('Missing');
		} elseif(!$_REQUEST["linktype"]) {
		  $error["linktype"] = $LANG->_('Missing');
		} elseif(($_REQUEST["linktype"] == 'file') and !$_REQUEST["filepath"]) {
		  // A file 'link' - must have path to file
		  $error["filepath"] = $LANG->_('Missing');
		} elseif(($_REQUEST["linktype"] == 'ldap') and (!$_REQUEST["ldaphost"] or !$_REQUEST["searchdn"])) {
		  // A ldap 'link' - must have host and DN
		  $error["ldapinfo"] = $LANG->_('Missing');
		}
	  } else {
		// Mountpoint below existing base mountpoint
		if(!@$_REQUEST["mountpoint"] or !@$_REQUEST["filesystem"]) {
		  // Must have mountpoint and filesystem 
		  $error["filesystem"] = $LANG->_('Missing');
		}
	  }

	  if(@$error) {
		echo "Error:"; printr($error);
		return false;
	  } else
		// All values ok - return true
		return true;
    } else {
	  // A page reload due to change in select
	  echo "Page reload due to select<br>";
	  return false;
	}
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
  global $_pql;

  // Setup list of filesystems
  $_REQUEST["FILESYSTEMS"] = array('ext2', 'ext3', 'reiserfs', 'xfs',
								   'msdos', 'umsdos', 'vfat', 'ntfs',
								   'iso9660', 'udf', 'ufs', 'smbfs',
								   'nfs', 'nfs4', 'ncpfs');

  $_REQUEST["CODEPAGES"] = array('ascii', 'base', 'cp1250', 'cp1251',
								 'cp1255', 'cp437', 'cp737', 'cp775',
								 'cp850', 'cp852', 'cp855', 'cp857',
								 'cp860', 'cp861', 'cp862', 'cp863',
								 'cp864', 'cp865', 'cp866', 'cp869',
								 'cp874', 'cp932', 'cp936', 'cp949',
								 'cp950', 'euc-jp', 'iso8859-1',
								 'iso8859-13', 'iso8859-14', 'iso8859-15',
								 'iso8859-2', 'iso8859-3', 'iso8859-4',
								 'iso8859-5', 'iso8859-6', 'iso8859-7',
								 'iso8859-9', 'koi8-r', 'koi8-ru',
								 'koi8-u', 'utf8');


  // {{{ Get all base mountpoints
  $dn = "ou=auto.master,".pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["host"].",".pql_get_define("PQL_CONF_SUBTREE_AUTOMOUNTS").",".$_REQUEST["domain"];
  $filter = '(&('.pql_get_define("PQL_ATTR_CN").'=*)(objectClass=autoMount))';
  if(!ereg('^/', $dn))
	$DNs = pql_get_dn($_pql->ldap_linkid, $dn, $filter, 'ONELEVEL');
	
  // Make sure we only get base mounts that are LDAP links
  for($i=0; $DNs[$i]; $i++) {
	$info = pql_get_attribute($_pql->ldap_linkid, $DNs[$i], pql_get_define("PQL_ATTR_AUTOMOUNT_INFO"));
	if(!eregi('ldap', $info))
	  unset($DNs[$i]);
  }

  // Uniquiefy the list of DN's.
  pql_uniq($DNs);

  $_REQUEST["DNs"] = $DNs;
// }}}

  if(@$_REQUEST["action"] == 'modify') {
	// {{{ We're called with a MODIFY command - special circumstances

	// Extract the mountpoint to modify
	$tmp = split(',', urldecode($_REQUEST["mount"]));
	$MOUNT_POINT = ereg_replace(pql_get_define("PQL_ATTR_CN").'=', '', $tmp[0]);

	// Retreive autoMount maps for this host
	$automounts = pql_get_automount_maps($_pql->ldap_linkid, $_REQUEST["host"]);

	// Find the automount map object for the specified mountpoint to modify
	// Also find the base mountpoint for that mountpoint.
	$MOUNTS = array('/');
	foreach($automounts as $mntpnt => $mntdata) {
	  if($mntdata["automountinformation"]) {
		// Device mounted on mountpoint
		if(lc($mntpnt) == lc($MOUNT_POINT)) {
		  $info = $mntdata;

		  // Retreive the base mountpoint for this automount
		  // The last MOUNTS array value will be this mountpoints
		  // base mount...
		  if(!@$_REQUEST["basemount"])
			$_REQUEST["basemount"] = $MOUNTS[count($MOUNTS)-1];

		  last;
		}
	  } else {
		// Mountpoint with more than one device

		// Add this basemount to the MOUNTS array.
		pql_add2array($MOUNTS, $mntpnt);

		foreach($mntdata as $mount => $mountdata) {
		  if($mountdata["automountinformation"]) {
			// Device mounted on mountpoint
			if(lc($mount) == lc($MOUNT_POINT)) {
			  $info = $mountdata;

			  // Retreive the base mountpoint for this automount
			  // The last MOUNTS array value will be this mountpoints
			  // base mount...
			  if(!@$_REQUEST["basemount"])
				$_REQUEST["basemount"] = $MOUNTS[count($MOUNTS)-1];

			  last;
			}
		  }
		}
	  }
	}
	echo "Basemount: '".$_REQUEST["basemount"]."'<br>";

	// Extract mountpoint
	if(!@$_REQUEST["mountpoint"]) {
	  $tmp = split(',', $info["dn"]);
	  $_REQUEST["mountpoint"]  = ereg_replace(pql_get_define("PQL_ATTR_CN").'=', '', $tmp[0]);
	}

	// Extract description and autoMountInfo(rmation) attributes
	if(!@$_REQUEST[pql_get_define("PQL_ATTR_DESCRIPTION")])
	  $_REQUEST[pql_get_define("PQL_ATTR_DESCRIPTION")]    = $info[pql_get_define("PQL_ATTR_DESCRIPTION")];
	if(!@$_REQUEST[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")])
	  $_REQUEST[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")];

	// Extract the device from the automountInfo attribute
	if(!@ $_REQUEST["device"])
	  $_REQUEST["device"] = ereg_replace('.* :', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
	
	// Extract the filesystem from the automountInfo attribute
	$tmp = preg_replace('/-fstype=/', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
	$opts = ereg_replace(' :.*', '', $tmp);
	$opts = split(',', $opts);
	$fs = $opts[0];

	// Setup the filesystem request variable
	if(!@$_REQUEST["filesystem"]) {
	  // Complicated because I haven't listed EVERY possible FS in
	  // the FILESYSTEMS array, so we need to do it in two steps. 
	  // 1. Get the exact match - all is well
	  foreach($_REQUEST["FILESYSTEMS"] as $fslist) {
		if($fslist == $fs)
		  $_REQUEST["filesystem"] = $fs;
	  }
	  
	  // ... (no exact match)
	  // 2. Setup so that we get 'New base mountpoint' and then use a 'input text' field
	  if(!@$_REQUEST["filesystem"]) {
		$_REQUEST["filesystem"] = 'new';
		$_REQUEST["realfilesystem"] = $fs;
	  } elseif($_REQUEST["filesystem"] == 'smbfs') {
		// Extract SMB server and share
		$tmp = ereg_replace('.*://', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
		$tmp = split('/', $tmp);
		
		$_REQUEST["smbserver"] = $tmp[0];
		$_REQUEST["smbshare"] = '';
		for($i=1; $tmp[$i]; $i++) {
		  $_REQUEST["smbshare"] .= $tmp[$i];
		  if(@$tmp[$i+1])
			$_REQUEST["smbshare"] .= '/';
		}
	  } elseif(($_REQUEST["filesystem"] == 'nfs') or ($_REQUEST["filesystem"] == 'nfs4')) {
		// Extract NFS server and 'share'
		$tmp = ereg_replace('.*://', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
		$tmp = split('/', $tmp);
		
		$_REQUEST["nfsserver"] = $tmp[0];
		$_REQUEST["nfsshare"] = '';
		for($i=1; $tmp[$i]; $i++) {
		  $_REQUEST["nfsshare"] .= $tmp[$i];
		  if(@$tmp[$i+1])
			$_REQUEST["nfsshare"] .= '/';
		}
	  }
	}

	// Setup the options request variable
	if(!@$_REQUEST["options"]) {
	  $_REQUEST["options"] = '';
	  for($i=1; $opts[$i]; $i++) {
		if($_REQUEST["filesystem"] == 'smbfs') {
		  // Extract SMB specific stuff
		  if(eregi('username', $opts[$i])) {
			$_REQUEST["smbcredtype"] = 'manual';
			$_REQUEST["smbcreduser"] = eregi_replace('username=', '', $opts[$i]);
		  } elseif(eregi('password', $opts[$i])) {
			$_REQUEST["smbcredtype"] = 'manual';
			$_REQUEST["smbcredpasswd"] = eregi_replace('password=', '', $opts[$i]);
		  } elseif(eregi('credentials', $opts[$i])) {
			$_REQUEST["smbcredtype"] = 'file';
			$_REQUEST["smbcredfile"] = eregi_replace('credentials=', '', $opts[$i]);
			
		  } elseif(eregi('uid', $opts[$i])) {
			$_REQUEST["smbcreduid"]  = eregi_replace('uid=', '', $opts[$i]);
		  } elseif(eregi('gid', $opts[$i])) {
			$_REQUEST["smbcredgid"]  = eregi_replace('gid=', '', $opts[$i]);
		  }
		} else {
		  $_REQUEST["options"] .= $opts[$i];
		  if($opts[$i+1])
			$_REQUEST["options"] .= ',';
		}
	  }
	}
// }}}
  }
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG, $_pql;

	attribute_init();

	if(!@$_REQUEST["codepage"])
	  $_REQUEST["codepage"] = 'cp437';
	if(!@$_REQUEST["iocharset"])
	  $_REQUEST["iocharset"] = 'iso8859-1';

	// So it's not urlencoded over and over and over and ...
	$_REQUEST["mount"] = urldecode($_REQUEST["mount"]);
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="POST" name="automount">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('automount'), 'domain' => $_REQUEST["orgname"])); ?>
<?php
	// {{{ Note about required options
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><font color="red">*</font></td>
          <td>=> <?=$LANG->_('Required options')?></td>
        </tr>

<?php
// }}}

	// {{{ Base mountpoint
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Base Mountpoint')?></td>
          <td><font color="red">*</font></td>
          <td>
            <select name="basemount" onChange="submit()">
<?php
	foreach($_REQUEST["DNs"] as $dn) {
	  $cn = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_CN"));
	  echo "              <option value=\"$cn\"";
	  if($_REQUEST["basemount"] == "$cn") {
		echo " SELECTED";
	  }
	  echo ">$cn</option>\n";
	}
?>
              <option value="new" <?php if($_REQUEST["basemount"] == 'new') { echo "SELECTED"; }?>>New base mountpoint</option>
            </select>
          </td>
        </tr>

<?php
// }}}

	// {{{ Mountpoint and Description
?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mountpoint')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="mountpoint" value="<?=$_REQUEST["mountpoint"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Description')?></td>
          <td></td>
          <td><input type="text" name="description" value="<?=$_REQUEST["description"]?>" size="60"></td>
        </tr>

<?php
// }}}

	if($_REQUEST["basemount"] == 'new') {
	  // {{{ New base mount
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Link type')?></td>								
          <td><font color="red">*</font></td>
          <td>
            <select name="linktype" onChange="submit()">>
              <option value="ldap" <?php if($_REQUEST["linktype"] == 'ldap') { echo "SELECTED"; }?>>LDAP</option>
              <option value="file" <?php if($_REQUEST["linktype"] == 'file') { echo "SELECTED"; }?>>File</option>
            </select>
          </td>
        </tr>

<?php if(!@$_REQUEST["linktype"] or ($_REQUEST["linktype"] == "ldap")) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('LDAP Host')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="ldaphost" value="<?=$_REQUEST["ldaphost"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Search DN')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="searchdn" value="<?=$_REQUEST["searchdn"]?>" size="60"></td>
        </tr>
<?php	} else { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('File path')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="filepath" value="<?=$_REQUEST["filepath"]?>"></td>
        </tr>
<?php	}
// }}}
	} else {
	  // Use existing base mountpoints
	  if($_REQUEST["filesystem"] == 'new') {
		if(@$_REQUEST["realfilesystem"])
		  $_REQUEST["filesystem"] = $_REQUEST["realfilesystem"];
		else
		  unset($_REQUEST["filesystem"]);

		// {{{ Choose filesystem
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Filesystem')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="filesystem" value="<?=$_REQUEST["filesystem"]?>"></td>
        </tr>
<?php } else { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Filesystem')?></td>
          <td><font color="red">*</font></td>
          <td>
            <select name="filesystem" onChange="submit()">
<?php	foreach($_REQUEST["FILESYSTEMS"] as $fs) { ?>
              <option value="<?=$fs?>" <?php if($_REQUEST["filesystem"] == $fs) { echo "SELECTED"; }?>><?=$fs?></option>
<?php	} ?>
              <option value="new"      <?php if($_REQUEST["filesystem"] == 'new')      { echo "SELECTED"; }?>>Other</option>
            </select>
          </td>
        </tr>

<?php
// }}}
	  }

	  // {{{ Stuff availible for multiple filesystems
	  // {{{ Mount owner (uid/gid)
	  if(($_REQUEST["filesystem"] == 'smbfs') or ($_REQUEST["filesystem"] == 'msdos') or ($_REQUEST["filesystem"] == 'umsdos') or
		 ($_REQUEST["filesystem"] == 'vfat')  or ($_REQUEST["filesystem"] == 'ntfs')  or ($_REQUEST["filesystem"] == 'iso9660') or
		 ($_REQUEST["filesystem"] == 'udf'))
	  {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mount owner (uid)')?></td>
          <td></td>
          <td><input type="text" name="creduid" value="<?=$_REQUEST["creduid"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mount owner (gid)')?></td>
          <td></td>
          <td><input type="text" name="credgid" value="<?=$_REQUEST["credgid"]?>"></td>
        </tr>

<?php
	  }
// }}}

	  // {{{ Block size
	  if(($_REQUEST["filesystem"] == 'msdos') or ($_REQUEST["filesystem"] == 'umsdos') or
		 ($_REQUEST["filesystem"] == 'vfat') or ($_REQUEST["filesystem"] == 'iso9660') or
		 ($_REQUEST["filesystem"] == 'udf'))
	  {
?>	  

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Blocksize')?></td>
                   <td></td>
                   <td>
                     <input type="radio" name="blocksize" value="512">512&nbsp;&nbsp;&nbsp;
                     <input type="radio" name="blocksize" value="1024">1024&nbsp;&nbsp;&nbsp;
                     <input type="radio" name="blocksize" value="2048">2048
                   </td>
                 </tr>

<?php
	  }
// }}}

	  // {{{ IO Character Set
	  if(($_REQUEST["filesystem"] == 'msdos') or ($_REQUEST["filesystem"] == 'umsdos') or
		 ($_REQUEST["filesystem"] == 'vfat')  or ($_REQUEST["filesystem"] == 'ntfs') or
		 ($_REQUEST["filesystem"] == 'iso9660') or ($_REQUEST["filesystem"] == 'udf'))
	  {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Character set to use when returning file names')?></td>
          <td></td>
          <td>
            <select name="iocharset">
<?php	foreach($_REQUEST["CODEPAGES"] as $iocharset) { ?>
              <option name="<?=$iocharset?>"<?php if($_REQUEST["iocharset"] == $iocharset) { echo " SELECTED"; } ?>><?=$iocharset?></option>
<?php	} ?>
            </select>
          </td>
        </tr>
<?php
	  }
// }}}

	  // {{{ Session to mount
	  if(($_REQUEST["filesystem"] == 'iso9660') or ($_REQUEST["filesystem"] == 'udf')) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mount session number')?></td>
          <td></td>
          <td><input type="text" name="session" value="<?=$_REQUEST["session"]?>" onChange="submit()"></td>
        </tr>
<?php
	  }
// }}}
	  // }}}

	  // {{{ Basic FS Options (value inputs)
	  if($_REQUEST["filesystem"] == 'smbfs') {
		// {{{ SMBFS
?>
        <!-- SMBFS -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMB server')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="smbserver" value="<?=$_REQUEST["smbserver"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMB share')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="smbshare" value="<?=$_REQUEST["smbshare"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMB Credential type')?></td>
          <td></td>
          <td>
            <select name="smbcredtype" onChange="submit()">
              <option value="guest"  <?php if(!@$_REQUEST["smbcredtype"] or ($_REQUEST["smbcredtype"] == 'guest')) { echo "SELECTED"; }?>>Guest</option>
              <option value="file"   <?php if($_REQUEST["smbcredtype"] == 'file') { echo "SELECTED"; }?>>File</option>
              <option value="manual" <?php if($_REQUEST["smbcredtype"] == 'manual') { echo "SELECTED"; }?>>Manual</option>
            </select>
          </td>
        </tr>
<?php	if($_REQUEST["smbcredtype"] == 'file') { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMB Credentials file')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="smbcredfile" value="<?=$_REQUEST["smbcredfile"]?>"></td>
        </tr>
<?php	} elseif($_REQUEST["smbcredtype"] == 'manual') { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMB Username')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="smbcreduser" value="<?=$_REQUEST["smbcreduser"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMB Password')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="smbcredpasswd" value="<?=$_REQUEST["smbcredpasswd"]?>"></td>
        </tr>
<?php	}
// }}}
	  } elseif(($_REQUEST["filesystem"] == 'nfs') or ($_REQUEST["filesystem"] == 'nfs4')) {
		// {{{ NFS/NFS4
?>
        <!-- NFS/NFS4 -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('NFS server')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="nfsserver" value="<?=$_REQUEST["nfsserver"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remote mount point')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="nfsshare" value="<?=$_REQUEST["nfsshare"]?>"></td>
        </tr>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'ncpfs') {
		// {{{ NCPFS
?>
        <!-- NCPFS -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Netware server')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="ncpserver" value="<?=$_REQUEST["ncpserver"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remote mount point')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="ncpshare" value="<?=$_REQUEST["ncpshare"]?>"></td>
        </tr>
<?php
// }}}
	  } elseif(($_REQUEST["filesystem"] == 'msdos') or ($_REQUEST["filesystem"] == 'umsdos') or
			   ($_REQUEST["filesystem"] == 'vfat')  or ($_REQUEST["filesystem"] == 'ntfs'))
	  {
		// {{{ FAT based

		// TODO: Missing options
		//	cvf_format=module
		//	cvf_option=option
?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Bitmask of the permissions that are not present')?></td>
          <td></td>
          <td><input type="text" name="umask" value="<?=$_REQUEST["umask"]?>"></td>
        </tr>
<?php	if(@$_REQUEST["filesystem"] != 'ntfs') { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('The bitmask applied to directories only')?></td>
          <td></td>
          <td><input type="text" name="fatdmask" value="<?=$_REQUEST["fatdmask"]?>"></td>
        </tr>
<?php	} ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('The bitmask applied to regular files only')?></td>
          <td></td>
          <td><input type="text" name="fatfmask" value="<?=$_REQUEST["fatfmask"]?>"></td>
        </tr>
<?php	if(@$_REQUEST["filesystem"] != 'ntfs') { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Codepage for converting to shortname characters')?></td>
          <td></td>
          <td>
            <select name="codepage">
<?php		foreach($_REQUEST["CODEPAGES"] as $codepage) { ?>
              <option name="<?=$codepage?>"<?php if($_REQUEST["codepage"] == $codepage) { echo " SELECTED"; } ?>><?=$codepage?></option>
<?php		} ?>
            </select>
          </td>
        </tr>
<?php	}
// }}}
	  } elseif(($_REQUEST["filesystem"] == 'ext2') or ($_REQUEST["filesystem"] == 'ext3')) {
		// {{{ EXT2 or EXT3
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Reserved blocks user (UID)')?></td>
                   <td></td>
                   <td><input type="text" name="reserveduid" value="<?=$_REQUEST["reserveduid"]?>"></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Reserved blocks user (GID)')?></td>
                   <td></td>
                   <td><input type="text" name="reservedgid" value="<?=$_REQUEST["reservedgid"]?>"></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Use this block instead of block 1 for superblock')?></td>
                   <td></td>
                   <td><input type="text" name="superblock" value="<?=$_REQUEST["superblock"]?>"></td>
                 </tr>
<?php	if($_REQUEST["filesystem"] == 'ext3') { ?>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Sync all data and metadata this often')?></td>
                   <td></td>
                   <td><input type="text" name="ext3_journal_commit" value="<?=$_REQUEST["ext3_journal_commit"]?>">&nbsp;<?=$LANG->_('seconds');?><td>
                 </tr>
												
<?php	} ?>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'reiserfs') {
		// {{{ Mount options for ReiserFS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Remount option which\npermits online expansion\nof reiserfs partitions')?></td>
                   <td></td>
                   <td><input type="text" name="resize" value="<?=$_REQUEST["resize"]?>"></td>
                 </tr>

<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'xfs') {
		// {{{ Mount options for XFS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Preferred buffered I/O size')?></td>
                   <td></td>
                   <td><input type="text" name="biosize" value="<?=$_REQUEST["biosize"]?>"></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Number of in-memory log buffers')?></td>
                   <td></td>
                   <td><input type="text" name="logbufs" onChange="submit()" value="<?=$_REQUEST["logbufs"]?>"></td>
                 </tr>
<?php	if(@$_REQUEST["logbufs"]) { ?>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Size of each in-memory log buffer')?></td>
                   <td></td>
                   <td><input type="text" name="logbsize" value="<?=$_REQUEST["logbsize"]?>"></td>
                 </tr>
<?php	} ?>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Use external metadata journal')?></td>
                   <td></td>
                   <td><input type="text" name="logdev" value="<?=$_REQUEST["logdev"]?>"></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Use external real-time device')?></td>
                   <td></td>
                   <td><input type="text" name="rtdev" value="<?=$_REQUEST["rtdev"]?>"></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Stripe unit')?></td>
                   <td></td>
                   <td><input type="text" name="sunit" value="<?=$_REQUEST["sunit"]?>"></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td class="title"><?=$LANG->_('Stripe width')?></td>
                   <td></td>
                   <td><input type="text" name="swidth" value="<?=$_REQUEST["swidth"]?>"></td>
                 </tr>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'iso9660') {
		// {{{ Mount options for ISO9660
		if(@$_REQUEST["norock"]) {
?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Give all files this mode (octal)')?></td>
          <td></td>
          <td><input type="text" name="mode" value="<?=$_REQUEST["mode"]?>"></td>
        </tr>
<?php	}

		if(@$_REQUEST["session"]) {
?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Session begins at sector')?></td>
          <td></td>
          <td><input type="text" name="session_start" value="<?=$_REQUEST["session_start"]?>"></td>
        </tr>
<?php	}
// }}}
	  } elseif($_REQUEST["filesystem"] == 'udf') {
		// {{{ Mount options for udf
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default bitmask (oct)')?></td>
          <td></td>
          <td><input type="text" name="umask" value="<?=$_REQUEST["umask"]?>"></td>
        </tr>
<?php
// }}}
	  } else {
		// {{{ Unspecified FS
?>
        <!-- OTHER FS TYPE -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Device')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="device" value="<?=$_REQUEST["device"]?>"></td>
        </tr>
<?php
// }}}
	  }
// }}}

	  // {{{ Additional FS Options (toggles or multiple choices)
?>
        <tr class="<?php pql_format_table(); ?>" valign="top">
          <td class="title"><?=$LANG->_('Mount Options')?></td>
          <td></td>
          <td>
            <table cellspacing="0" cellpadding="0" border="0" width="100%">
               <th colspan="2" align="left">
<?php
	  if(($_REQUEST["filesystem"] == 'ext2') or ($_REQUEST["filesystem"] == 'ext3') or !@$_REQUEST["filesystem"]) {
		// {{{ Mount options for ext2 or ext3
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext2_acl"><?=$LANG->_('Support POSIX Access Control Lists')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="ext2_check"><?=$LANG->_('Check filesystem at every mount time')?>&nbsp;&nbsp;</td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext2_nobh"><?=$LANG->_('Do not attach buffer_heads to file pagecache')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="ext2_nouid32"><?=$LANG->_('Disables 32-bit UIDs and GIDs')?>&nbsp;&nbsp;</td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext2_debug"><?=$LANG->_('Print debugging info upon each (re)mount')?></td>
                   <td><input type="checkbox" name="ext2_xattr"><?=$LANG->_('Support "user." extended attributes')?></td>
                 </tr>

<?php	if($_REQUEST["filesystem"] == 'ext3') { ?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext3_noload"><?=$LANG->_("Do not load the ext3 file system's journal on mounting")?></td>
                   <td><input type="checkbox" name="ext3_journal_update"><?=$LANG->_("Update the ext3 file system's journal to the current format")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('journalling mode for file data')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="error" value="journal"><?=$LANG->_('All data is committed into the journal prior to being written into the main file system.')?>
                   </td>

                   <td>
                     <input type="radio" name="error" value="ordered"><?=$LANG->_('All data is forced directly out to the main file system prior to its metadata being committed to the journal.')?><br>
                     <input type="radio" name="error" value="writeback"><?=$LANG->_('Data ordering is not preserved - data may be written into the main file system after its metadata has been committed to the journal.')?>
                   </td>
                 </tr>

<?php	} ?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Set the behaviour for the statfs system call')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="statfs" value="bsddf"><?=$LANG->_("Makes 'df' act like on BSD")?>
                   </td>
                   <td><br><input type="radio" name="statfs" value="minixdf"><?=$LANG->_("Makes 'df' act like on Minix")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('What group id will a newly created file gets')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="groupid" value="bsdgroups"><?=$LANG->_("Give objects the same group ID as their parent")?>
                   </td>
                   <td><br><input type="radio" name="groupid" value="sysvgroups"><?=$LANG->_("New objects have the group ID of their creator")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Allocator to use for new inode')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="allocator" value="oldalloc"><?=$LANG->_("Old allocator")?>
                   </td>
                   <td><br><input type="radio" name="allocator" value="orlov"><?=$LANG->_("Orlov allocator")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Define the behaviour when an error is encountered')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="error" value="continue"><?=$LANG->_('Keep going on a filesystem error')?>
                   </td>
                   <td><input type="radio" name="error" value="remount"><?=$LANG->_('Remount the filesystem read-only on an error')?><br>
                       <input type="radio" name="error" value="panic"><?=$LANG->_('Panic and halt the machine if an error occurs')?>
                   </td>
                 </tr>
<?php
// }}}
	  } elseif(($_REQUEST["filesystem"] == 'msdos') or ($_REQUEST["filesystem"] == 'umsdos') or ($_REQUEST["filesystem"] == 'vfat')) {
		// {{{ Mount options for msdos, umsdos and/or vfat
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="fat_debug"><?=$LANG->_('Print debugging info upon each (re)mount')?></td>
                   <td><input type="checkbox" name="fat_quiet"><?=$LANG->_('Attempts to chown or chmod files do not return errors, although they fail')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Text format conversion')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="textconv" value="binary"><?=$LANG->_('No translation')?>
                   </td>
                   <td>
                     <input type="radio" name="textconv" value="text"><?=$LANG->_('CRLF<-->NL translation')?><br>
                     <input type="radio" name="textconv" value="auto"><?=$LANG->_('CRLF<-->NL translation on all files not having "well-known binary" extension')?>
                   </td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Check pickyness')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="check" value="relaxed"><?=$LANG->_('Relaxed')?>
                   </td>
                   <td><input type="radio" name="check" value="normal"><?=$LANG->_('Normal')?><br>
                       <input type="radio" name="check" value="strict"><?=$LANG->_('Strict')?>
                   </td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('FAT Type')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="fattype" value="12">12
                   </td>
                   <td><input type="radio" name="fattype" value="16">16
                       <input type="radio" name="fattype" value="32">32
                   </td>
                 </tr>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'ntfs') {
		// {{{ Mount options for NTFS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="utf8"><?=$LANG->_('Use UTF-8 for converting file names')?></td>
                   <td><input type="checkbox" name="posix"><?=$LANG->_('Distinguishes between upper and lower case')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Use escape sequences for unknown Unicode characters')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="uni_xlate" value="1"><?=$LANG->_('True')?>
                   </td>
                   <td>
                     <input type="radio" name="uni_xlate" value="2"><?=$LANG->_('Use VFAT style 4byte escape sequences')?><br>
                     <input type="radio" name="uni_xlate" value="0"><?=$LANG->_('False')?>
                   </td>
                 </tr>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'reiserfs') {
		// {{{ Mount options for reiserfs
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_conv"><?=$LANG->_('Instructs version 3.6 reiserfs software to mount a version 3.5 FS')?></td>
                   <td><input type="checkbox" name="reiser_hashreloc"><?=$LANG->_('Tunes the block allocator')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_nounhashreloc"><?=$LANG->_('Tunes the block allocator')?></td>
                   <td><input type="checkbox" name="reiser_noborder"><?=$LANG->_('Disable the border allocator algorithm')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_nolog"><?=$LANG->_('Disable journalling')?></td>
                   <td><input type="checkbox" name="reiser_notail"><?=$LANG->_("Disable packing small files and 'file tails' in the tree")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_replayonly"><?=$LANG->_('Replay the transactions which are in the journal, but do actually mount the file system')?></td>
                   <td></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Hash function to use for finding files in directories')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="hash" value="rupasov">rupasov&nbsp;&nbsp;&nbsp;
                     <input type="radio" name="hash" value="tea">tea
                   </td>
                   <td>
                       <input type="radio" name="hash" value="r5">r5<br>
                       <input type="radio" name="hash" value="detect">detect
                   </td>
                 </tr>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'xfs') {
		// {{{ Mount options for XFS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="dmapi"><?=$LANG->_('Enable the DMAPI event callouts')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="noalign"><?=$LANG->_('Do not aligned data allocations at stripe unit boundaries')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="noatime"><?=$LANG->_('Do not update access timestamps')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="norecovery"><?=$LANG->_('Mount filesystem without running log recovery')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="nouuid"><?=$LANG->_('Ignore the filesystem uuid')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="osyncisdsync"><?=$LANG->_('Make writes to files opened with the O_SYNC flag set behave\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;as if the O_DSYNC flag had been used instead.')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="usrquota" onChange="submit()"><?=$LANG->_('Enable user quota')?>&nbsp;&nbsp;</td>
<?php	if(@$_REQUEST["usrquota"]) { ?>
                   <td><input type="checkbox" name="uqnoenforce"><?=$LANG->_('Enforce user quota limits')?></td>
<?php	} ?>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="grpquota" onChange="submit()"><?=$LANG->_('Enable group quota')?>&nbsp;&nbsp;</td>
<?php	if(@$_REQUEST["grpquota"]) { ?>
                   <td><input type="checkbox" name="gqnoenforce"><?=$LANG->_('Enforce group quota limits')?></td>
<?php	} ?>
                 </tr>
<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'iso9660') {
		// {{{ Mount options for ISO9660
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="norock" onChange="submit()"><?=$LANG->_('Disable the use of Rock Ridge extensions')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="nojoliet"><?=$LANG->_('Disable the use of Microsoft Joliet extensions')?></td>
                 </tr>

<?php	if(@$_REQUEST["norock"]) { ?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <?=$LANG->_('Type of filename lookup')?>&nbsp;&nbsp;<br>
                     <input type="radio" name="check" value="relaxed"><?=$LANG->_('Relaxed')?>&nbsp;&nbsp;&nbsp;
                     <input type="radio" name="check" value="strict"><?=$LANG->_('Strict')?>
                   </td>
                   <td></td>
                 </tr>
<?php	} ?>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="unhide"><?=$LANG->_('Show hidden and associated files')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="cruft"><?=$LANG->_('Ignore the high order bits of the file length')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="utf8"><?=$LANG->_('Use UTF-8 for converting file names')?></td>
                   <td></td>
                 </tr>

<?php
// }}}
	  } elseif($_REQUEST["filesystem"] == 'udf') {
		// {{{ Mount options for udf
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="unhide"><?=$LANG->_('Show otherwise hidden files')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="undelete"><?=$LANG->_('Show deleted files in lists')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="nostrict"><?=$LANG->_('Unset strict conformance')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="novrs"><?=$LANG->_('Skip volume sequence recognition')?>&nbsp;&nbsp;</td>
                 </tr>
<?php
// }}}
	  } else {
		// {{{ Mount options for unspecified FS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td>&nbsp;
                     <input type="text" name="options" value="<?=$_REQUEST["options"]?>"><br>
                     &nbsp;&nbsp;NOTE: Currently you will have to enter this<br>
                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;manually (using the manpage(s)<br>
                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for your filesystem).<br>

                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Some day, I'll add radio buttons for<br>
                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;you to click here...<br>

                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Please separate your options with comma.
                   </td>
                 </tr>
<?php
// }}}
	  }
?>
               </th>
             </table>
          </td>
        </tr>
<?php
// }}}
	}
?>
      </th>
    </table>

<?php
	// {{{ Hidden inputs
?>
    <input type="hidden" name="dosave"   id="dosave">
    <input type="hidden" name="submit"   value="<?=$_REQUEST["submit"]?>">
    <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
    <input type="hidden" name="host"     value="<?=$_REQUEST["host"]?>">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=urlencode($_REQUEST[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")])?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
<?php if(@$_REQUEST["mount"]) { ?>
    <input type="hidden" name="mount"    value="<?=urlencode($_REQUEST["mount"])?>">
<?php } ?>
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="button" value="<?=$LANG->_('Save')?>" onClick="document.automount.dosave.value='yes'; submit();">
  </form>
<?php
// }}}
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
	echo "attribute_save($type)<br>";

    switch($type) {
      case "add":
		// {{{ Add a automount
		// {{{ Setup DN
		$dn  = pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["basemount"].",ou=auto.master,".pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["host"].",";
		$dn .= pql_get_define("PQL_CONF_SUBTREE_AUTOMOUNTS").",".$_REQUEST["domain"];
		$info = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_AUTOMOUNT_INFO"));
		if(eregi('ldap ', $info)) {
		  // A LDAP link - asume that the DN is in this server
		  // (simply because I won't be able to connect to another server here)
		  $dn = split(':', $info); $dn = $dn[1];
		} elseif(eregi('file ', $info)) {
		  // A file link - can't follow that!
?>
Sorry, this base mount is a <b>file</b> link and I don't know how to follow that...
<?php
		} else {
		  // Don't know what to do now... Die!
		  $object = pql_search($_pql->ldap_linkid, $dn, 'objectClass=*', BASE);
?>
Sorry, this base mount is not an LDAP link. Don't know what to do now...<br>
Please report this to either to the <a href="http://bugs.bayour.com/" target="_new">bugtracker</a>
or to the <a href="mailto:phpqladmin@lists.bayour.com">mailinglist</a><br>
Make sure you include this object information (I'm mostly interested in the <b><?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?></b> attribute):
<?php
		  printr($object);
		  die();
		}

		// Prefix the DN with the name of the mounpoint
		$dn = pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["mountpoint"].",$dn";
// }}}

		// {{{ Setup object
		$object["dn"] = $dn;
		$object[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'autoMount';
		if(@$_REQUEST["description"])
			$object[pql_get_define("PQL_ATTR_DESCRIPTION")] = $_REQUEST["description"];
		if(@$_REQUEST["mountpoint"])
			$object[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["mountpoint"];

		$object[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = attribute_setup();
// }}}

		// {{{ Add object to LDAP server
		// The ':_add' is only so that pql_create_ldif() will not put the 'replace: ...' lines in the LDIF.
		if(pql_write_add($_pql->ldap_linkid, $dn, $object, 'automount', 'attrib.automount.inc:attribute_save -> _add'))
		  $msg = $LANG->_('Successfully added automount');
		else
		  $msg = $LANG->_('Failed to add automount').": ".ldap_error($_pql->ldap_linkid);
// }}}

		// Redirect to correct domain view page
		attribute_forward($msg);
		break;
// }}}

      case "modify":
		// {{{ Modify a automount
		// {{{ Setup (new) automountInformation attribute
		$dn = $_REQUEST["mount"];
		$automount = attribute_setup();
// }}}

		// {{{ Do modification
		if($automount == $_REQUEST["oldvalue"])
			// Don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Automount information')));
		else {
			if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_AUTOMOUNT_INFO"), 1, $automount))
				$msg = pql_complete_constant($LANG->_('Successfully changed %what% to SMB mount %new%'),
											 array('what' => $LANG->_('path'),
												   'new'  => $_REQUEST["mountpoint"]));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('path'))) . ": " . ldap_error($_pql->ldap_linkid);
		}
// }}}

		attribute_forward($msg);
		break;
// }}}

      case "delete":
		// {{{ Delete the automount information in $_REQUEST["mount"]
		if(pql_write_del($_pql->ldap_linkid, $_REQUEST["mount"]))
		  $msg = pql_complete_constant($LANG->_('Successfully removed mountpoint %mount%'),
									   array('mount' => $_REQUEST["mount"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove mountpoint %mount%'),
									   array('mount' => $_REQUEST["mount"])) . ": " . ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
// }}}
		
      default:
		// {{{ Unrecognized save type
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
// }}}
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
