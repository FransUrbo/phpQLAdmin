<?php
// attribute plugin for automount maps
// $Id: attrib.automount.inc,v 2.9 2007-07-19 10:16:22 turbo Exp $

// {{{ attribute_find_automaster(hostdn)
// Called from attribute_init() and attribute_save() to find
// the auto.master object for specific host DN.
function attribute_find_automaster($hostdn) {
  global $_pql;

  // Find the 'auto.master' object DN
  $auto_master = $_pql->get_dn($hostdn, 'ou=auto.master');
  if(!$auto_master[0])
	// No auto.master!? Let's wing it!!
	$auto_master[0] = 'ou=auto.master,'.$hostdn;

  return($auto_master[0]);
}
// }}}

// {{{ attribute_find_mountpoints(mnt_pnt, mnt_dn)
// Called from attribute_init() and will recursivly
// call itself to find ALL mountpoints which isn't a
// device mounted on a mountpoint.
// I.e. All objects that links to another object in LDAP.
function attribute_find_mountpoints($mnt_pnt, $mnt_dn) {
  global $_pql, $extra_dns;

  $info = $_pql->get_attribute($mnt_dn, pql_get_define("PQL_ATTR_AUTOMOUNT_INFO"));
  if(eregi('ldap', $info)) {
	// Get the mountpoint for the FS
	$sub_mnt = split(',', $mnt_dn);
	$sub_mnt = split('=', $sub_mnt[0]);
	if(!ereg('^/', $sub_mnt[1]))
	  $sub_mnt = '/'.$sub_mnt[1];
	else
	  $sub_mnt = $sub_mnt[1];

	// Remember this submount point link
	$extra_dns["$mnt_pnt$sub_mnt"] = $mnt_dn;

	// Since this is an LDAP link, find possible submounts
	$dn = split(':', $info);
	if($dn[2])
	  // info => 'ldap ldapi://%2fvar%2frun%2fslapd%2fldapi.provider:ou=auto.mnt,cn=pumba.bayour.com,ou=Computers,c=SE'
	  $dn = $dn[2];
	elseif($dn[1])
	  // info => 'ldap localhost:ou=auto.share,cn=pumba.bayour.com,ou=Computers,c=SE'
	  $dn = $dn[1];
	
	// Find any LDAP links below this DN
	$filter = '(&('.pql_get_define("PQL_ATTR_CN").'=*)(objectClass=autoMount))';
	$links = $_pql->get_dn($dn, $filter, 'ONELEVEL');
	for($i=0; $links[$i]; $i++)
	  // Is there more submount links after this?
	  attribute_find_mountpoints($mnt_pnt.$sub_mnt, $links[$i]);
  }
//  else
//	unset($DNs[$mnt]);
}
// }}}

// {{{ attribute_setup(void)
// Called from attribute_save() to setup the
// automountInformation attribute
function attribute_setup() {
  if($_REQUEST["filesystem"])
	$automount = "-fstype=".$_REQUEST["filesystem"];

  if(@$_REQUEST["options"])
	$automount .= ",".$_REQUEST["options"];

  if(($_REQUEST["basemount"] == 'new') or ($_REQUEST["filesystem"] == 'autofs')) {
	// {{{ New base mount point
	global $_pql;

	// {{{ Setup the 'auto.<mnt>' DN
	$mnt  = pql_get_define("PQL_ATTR_OU").'=auto.'.preg_replace('/\//', '', $_REQUEST["mountpoint"]);
	$mnt .= ','.$_REQUEST["host"];
// }}}

	$automount = $_REQUEST["linktype"]." ".$_REQUEST["ldaphost"].":".$mnt;
// }}}

  } elseif($_REQUEST["filesystem"] == 'smbfs') {
	// {{{ FS: SMB
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
	// }}}

  } elseif(($_REQUEST["filesystem"] == 'nfs') or ($_REQUEST["filesystem"] == 'nfs4')) {
	// {{{ FS: NFS/NFS4
	$automount .= " ".$_REQUEST["nfsserver"];
	if(!ereg('^/', $_REQUEST["nfsshare"]))
	  $automount .= "/";
	$automount .= ":".$_REQUEST["nfsshare"];
	// }}}

  } elseif($_REQUEST["filesystem"] == 'ncpfs') {
	// {{{ FS: NCP
	$automount .= " ".$_REQUEST["ncpserver"];
	if(!ereg('^/', $_REQUEST["ncpshare"]))
	  $automount .= "/";
	$automount .= ":".$_REQUEST["ncpshare"];
	// }}}

  } elseif($_REQUEST["filesystem"] == 'iso9660') {
	// {{{ FS: ISO9660
	// TODO: I'm forcing iso9660 options 'ro,sync,nodev,nosuid' for the time being.
	$automount .= ',ro,sync,nodev,nosuid';

	if($_REQUEST["utf8"])					$automount .= ",utf8";
	if($_REQUEST["creduid"])				$automount .= ",uid=".$_REQUEST["creduid"];
	if($_REQUEST["credgid"])				$automount .= ",gid=".$_REQUEST["credgid"];
	if($_REQUEST["session"])				$automount .= ",session=".$_REQUEST["session"];
	if($_REQUEST["iocharset"] and ($_REQUEST["iocharset"] != 'iso8859-1'))
	  // Not the default - use choosen value
	  $automount .= ",iocharset=".$_REQUEST["iocharset"];
// }}}

  } elseif(($_REQUEST["filesystem"] == 'ext2') or ($_REQUEST["filesystem"] == 'ext3')) {
	// {{{ FS: EXT2/EXT3
	if($_REQUEST["reserveduid"])			$automount .= ",resuid=".$_REQUEST["reserveduid"];
	if($_REQUEST["reservedgid"])			$automount .= ",resuid=".$_REQUEST["reservedgid"];
	if($_REQUEST["superblock"])				$automount .= ",".$_REQUEST["superblock"];

	if($_REQUEST["ext2_acl"])				$automount .= ",acl";
	if($_REQUEST["ext2_check"])				$automount .= ",check";
	if($_REQUEST["ext2_nobh"])				$automount .= ",nobh";
	if($_REQUEST["ext2_nouid32"])			$automount .= ",nouid32";
	if($_REQUEST["ext2_debug"])				$automount .= ",debug";
	if($_REQUEST["ext2_xattr"])				$automount .= ",user_xattr";
	if($_REQUEST["statfs"])					$automount .= ",".$_REQUEST["statfs"];
	if($_REQUEST["error"])					$automount .= ",errors=".$_REQUEST["error"];
	if($_REQUEST["allocator"])				$automount .= ",".$_REQUEST["allocator"];
	if($_REQUEST["groupid"])				$automount .= ",".$_REQUEST["groupid"];

	if($_REQUEST["ext3_noload"])			$automount .= ",noload";
	if($_REQUEST["ext3_journal_update"])	$automount .= ",journal=update";
	if($_REQUEST["ext3_journal_commit"])	$automount .= ",commit=".$_REQUEST["ext3_journal_commit"];
	if($_REQUEST["jmode"])					$automount .= ",data=".$_REQUEST["jmode"];
	// }}}
  }

  if($_REQUEST["device"]) {
	// {{{ FS: Other ('normal' mountpoint)
	$automount .= " :";
	$automount .= $_REQUEST["device"];
	// }}}
  }

  return $automount;
}
// }}}

// {{{ attribute_check(void)
// Called by host_edit_attribute.php before saving object.
function attribute_check() {
    global $error, $LANG;
    
	if($_REQUEST["dosave"] == 'yes') {
	  // We've clicked the Save button
	  if(($_REQUEST["basemount"] == 'new') or ($_REQUEST["filesystem"] == 'autofs')) {
		// {{{ Create a new base mountpoint
		if(!$_REQUEST["mountpoint"])
		  $error["mountpoint"] = $LANG->_('Missing');

		if($_REQUEST["mountpoint"] and !ereg('^/', $_REQUEST["mountpoint"]))
		  $_REQUEST["mountpoint"] = '/'.$_REQUEST["mountpoint"];

		if(!$_REQUEST["linktype"])
		  $error["linktype"] = $LANG->_('Missing');

		if(($_REQUEST["linktype"] == 'file') and !$_REQUEST["filepath"])
		  // A file 'link' - must have path to file
		  $error["filepath"] = $LANG->_('Missing');

		if(($_REQUEST["linktype"] == 'ldap') and !$_REQUEST["ldaphost"])
		  // A ldap 'link' - must have host and DN
		  $error["ldaphost"] = $LANG->_('Missing');

		// }}}

	  } else {
		// {{{ Mountpoint below existing base mountpoint
		if(!@$_REQUEST["mountpoint"] or !@$_REQUEST["filesystem"]) {
		  // Must have mountpoint and filesystem 
		  $error["filesystem"] = $LANG->_('Missing');
		}
		// }}}

	  }

	  // {{{ Filesystem which uses device
	  if((($_REQUEST["filesystem"] == 'msdos')    or ($_REQUEST["filesystem"] == 'umsdos') or
		  ($_REQUEST["filesystem"] == 'vfat')     or ($_REQUEST["filesystem"] == 'ntfs')   or
		  ($_REQUEST["filesystem"] == 'ext2')     or ($_REQUEST["filesystem"] == 'ext3')   or
		  ($_REQUEST["filesystem"] == 'reiserfs') or ($_REQUEST["filesystem"] == 'xfs')    or
		  ($_REQUEST["filesystem"] == 'iso9660')  or ($_REQUEST["filesystem"] == 'udf'))   and
		 !$_REQUEST["device"])
		$error["device"] = $LANG->_('Missing');
	  // }}}

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
// Called in attribute_print_form() to retreive all basic mount
// points for a specific host.
function attribute_init() {
  global $_pql, $extra_dns;

  // {{{ Setup full list of availible filesystems and codepages
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
  // }}}

  if(@$_REQUEST["host"]) {
	// {{{ Find the physical hosts auto.master object
	// Find the auto.master DN
	$dn = attribute_find_automaster($_REQUEST["host"]);
	// }}}

	// {{{ Get all base mountpoints
	if(@$dn and $_pql->get_dn($dn, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*')) {
	  // I.e. everything below the auto.master DN
	  $filter = '(&('.pql_get_define("PQL_ATTR_CN").'=*)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=autoMount))';
	  if(!ereg('^/', $dn))
		$DNs = $_pql->get_dn($dn, $filter, 'ONELEVEL');
	  
	  // Rearrange the array based on the actual mountpoint in the FS
	  for($i=0; $DNs[$i]; $i++) {
		$mnt = split(',', $DNs[$i]);
		$mnt = split('=', $mnt[0]);
		$new_array[$mnt[1]] = $DNs[$i];
	  }
	  $DNs = $new_array;
	  
	  // Make sure we only get base mounts that are LDAP links
	  $extra_dns = array();
	  foreach($DNs as $mnt_pnt => $mnt_dn)
		// Don't call with first parameter - that's retreived in the function.
		attribute_find_mountpoints('', $mnt_dn);
	  
	  if(is_array($extra_dns))
		$DNs = $extra_dns;
	  
	  // Sort the list of DN's on the mountpoint.
	  ksort($DNs);
	  
	  $_REQUEST["DNs"] = $DNs;
	}
	// }}}
  }

  // {{{ Retreive all physical hosts
  $hosts = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
						 '(&('.pql_get_define("PQL_ATTR_CN").'=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))',
						 'ONELEVEL');
  for($i=0; $hosts[$i]; $i++) {
	$cn = $_pql->get_attribute($hosts[$i], pql_get_define("PQL_ATTR_CN"));
	$_REQUEST["HOSTS"][$cn] = $hosts[$i];
  }
  // }}}

  if(@$_REQUEST["action"] == 'modify') {
	// {{{ We're called with a MODIFY command - find the base mountpoint and (sub)mountpoint

	// Fetch the automountInformation value from the object
	$info = $_pql->search($_REQUEST["mount"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE');

	// {{{ Figure out the basemount and the final mountpoint for this mount
	if(!@$_REQUEST["basemount"]) {
	  $_REQUEST["basemount"] = '/';
	  $tmp = split('/', $_REQUEST["mountpoint"]);
	  for($i=1; $tmp[$i]; $i++) {
		$_REQUEST["basemount"] .= $tmp[$i];
		if(!@$tmp[$i+2])
		  break;
		else
		  $_REQUEST["basemount"] .= '/';
	  }

	  // What's left of the $tmp array is the actual mountpoint 
	  $_REQUEST["mountpoint"] = $tmp[$i+1];
	}
	// }}}

	// {{{ Extract description and autoMountInfo(rmation) attributes
	if(!@$_REQUEST[pql_get_define("PQL_ATTR_DESCRIPTION")])
	  $_REQUEST[pql_get_define("PQL_ATTR_DESCRIPTION")]    = $info[pql_get_define("PQL_ATTR_DESCRIPTION")];
	if(!@$_REQUEST[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")])
	  $_REQUEST[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")];
	// }}}

	// {{{ Extract the device from the automountInfo attribute
	if(!@ $_REQUEST["device"])
	  $_REQUEST["device"] = ereg_replace('.* :', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
	// }}}

	// {{{ Extract the filesystem and mount options from the automountInfo attribute
	$tmp = preg_replace('/-fstype=/', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
	$opts = ereg_replace(' :.*', '', $tmp);
	$opts = split(',', $opts);
	$fs = $opts[0];
	// }}}

	// {{{ Setup the filesystem request variable
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
		// {{{ Extract SMB server and share
		$tmp = ereg_replace('.*://', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
		$tmp = split('/', $tmp);
		
		$_REQUEST["smbserver"] = $tmp[0];
		$_REQUEST["smbshare"] = '';
		for($i=1; $tmp[$i]; $i++) {
		  $_REQUEST["smbshare"] .= $tmp[$i];
		  if(@$tmp[$i+1])
			$_REQUEST["smbshare"] .= '/';
		}
		// }}}

	  } elseif(($_REQUEST["filesystem"] == 'nfs') or ($_REQUEST["filesystem"] == 'nfs4')) {
		// {{{ Extract NFS server and 'share'
		$tmp = ereg_replace('.*://', '', $info[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);
		$tmp = split('/', $tmp);
		
		$_REQUEST["nfsserver"] = $tmp[0];
		$_REQUEST["nfsshare"] = '';
		for($i=1; $tmp[$i]; $i++) {
		  $_REQUEST["nfsshare"] .= $tmp[$i];
		  if(@$tmp[$i+1])
			$_REQUEST["nfsshare"] .= '/';
		}
		// }}}

	  }
	} elseif($_REQUEST["filesystem"] == 'new') {
	  $_REQUEST["realfilesystem"] = $fs;
	}
	// }}}

	// {{{ Setup the options request variable
	if(!@$_REQUEST["options"]) {
	  $_REQUEST["options"] = '';
	  for($i=1; $opts[$i]; $i++) {
		if($_REQUEST["filesystem"] == 'smbfs') {
		  // {{{ Extract SMB specific stuff
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
		  // }}}

		} else {
		  $_REQUEST["options"] .= $opts[$i];
		  if($opts[$i+1])
			$_REQUEST["options"] .= ',';
		}
	  }
	}
// }}}
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

	if(!@$_REQUEST["filesystem"])
	  $_REQUEST["filesystem"] = 'ext3';
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

	// {{{ Hosts to add automount to
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><font color="red">*</font></td>
          <td>
            <select name="host" onChange="submit()">
<?php
	foreach($_REQUEST["HOSTS"] as $fqdn => $dn) {
	  echo "              <option value=\"$dn\"";
	  if($_REQUEST["host"] == $dn) {
		echo " SELECTED";
	  }
	  echo ">$fqdn</option>\n";
	}
?>
            </select>
          </td>
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
	foreach($_REQUEST["DNs"] as $mnt => $dn) {
	  echo "              <option value=\"$dn\"";
	  if(($_REQUEST["basemount"] == $dn) or ($_REQUEST["basemount"] == $mnt)) {
		// * If 'basemount' contains a DN, then we have changed the Base mountpoint select
		//   and the page got reloaded.
		// * If 'basemount' contains a mountpoint value and not a DN, then we're called
		//   from tables/host_details-automount.inc with a mount which is mounted below
		//   a submount...
		echo " SELECTED";
	  }
	  echo ">$mnt</option>\n";
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
              <option value="autofs"   <?php if($_REQUEST["filesystem"] == 'autofs')   { echo "SELECTED"; }?>><?=$LANG->_('Submount')?></option>
              <option value="new"      <?php if($_REQUEST["filesystem"] == 'new')      { echo "SELECTED"; }?>><?=$LANG->_('Other')?></option>
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

	  }

	  // {{{ Filesystem which uses device
	  if(($_REQUEST["filesystem"] == 'msdos')    or ($_REQUEST["filesystem"] == 'umsdos') or
		 ($_REQUEST["filesystem"] == 'vfat')     or ($_REQUEST["filesystem"] == 'ntfs')   or
		 ($_REQUEST["filesystem"] == 'ext2')     or ($_REQUEST["filesystem"] == 'ext3')   or
		 ($_REQUEST["filesystem"] == 'reiserfs') or ($_REQUEST["filesystem"] == 'xfs')    or
		 ($_REQUEST["filesystem"] == 'iso9660')  or ($_REQUEST["filesystem"] == 'udf'))
	  {
?>
        <!-- OTHER FS TYPE -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Device')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="device" value="<?=$_REQUEST["device"]?>"></td>
        </tr>
<?php
	  }
// }}}
// }}}

	  if($_REQUEST["filesystem"] == 'autofs') {
		// {{{ Submount
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
<?php   } else { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('File path')?></td>
          <td><font color="red">*</font></td>
          <td><input type="text" name="filepath" value="<?=$_REQUEST["filepath"]?>"></td>
        </tr>
<?php   }
// }}}

	  } else {
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
                   <td><input type="checkbox" name="ext2_acl"<?php if($_REQUEST["ext2_acl"]) { echo " CHECKED"; } ?>><?=$LANG->_('Support POSIX Access Control Lists')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="ext2_check"<?php if($_REQUEST["ext2_check"]) { echo " CHECKED"; } ?>><?=$LANG->_('Check filesystem at every mount time')?>&nbsp;&nbsp;</td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext2_nobh"<?php if($_REQUEST["ext2_nobh"]) { echo " CHECKED"; } ?>><?=$LANG->_('Do not attach buffer_heads to file pagecache')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="ext2_nouid32"<?php if($_REQUEST["ext2_nouid32"]) { echo " CHECKED"; } ?>><?=$LANG->_('Disables 32-bit UIDs and GIDs')?>&nbsp;&nbsp;</td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext2_debug"<?php if($_REQUEST["ext2_debug"]) { echo " CHECKED"; } ?>><?=$LANG->_('Print debugging info upon each (re)mount')?></td>
                   <td><input type="checkbox" name="ext2_xattr"<?php if($_REQUEST["ext2_xattr"]) { echo " CHECKED"; } ?>><?=$LANG->_('Support "user." extended attributes')?></td>
                 </tr>

<?php	if($_REQUEST["filesystem"] == 'ext3') { ?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="ext3_noload"<?php if($_REQUEST["ext3_noload"]) { echo " CHECKED"; } ?>><?=$LANG->_("Do not load the ext3 file system's journal on mounting")?></td>
                   <td><input type="checkbox" name="ext3_journal_update"<?php if($_REQUEST["ext3_journal_update"]) { echo " CHECKED"; } ?>><?=$LANG->_("Update the ext3 file system's journal to the current format")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Journalling mode for file data')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="jmode" value="journal"<?php if($_REQUEST["error"] == 'journal') { echo " CHECKED"; } ?>><?=$LANG->_('All data is committed into the journal prior to being written into the main file system.')?>
                   </td>

                   <td>
                     <input type="radio" name="jmode" value="ordered"<?php if($_REQUEST["error"] == 'ordered') { echo " CHECKED"; } ?>><?=$LANG->_('All data is forced directly out to the main file system prior to its metadata being committed to the journal.')?><br>
                     <input type="radio" name="jmode" value="writeback"<?php if($_REQUEST["error"] == 'writeback') { echo " CHECKED"; } ?>><?=$LANG->_('Data ordering is not preserved - data may be written into the main file system after its metadata has been committed to the journal.')?>
                   </td>
                 </tr>

<?php	} ?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Set the behaviour for the statfs system call')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="statfs" value="bsddf"<?php if($_REQUEST["statfs"] == 'bsddf') { echo " CHECKED"; } ?>><?=$LANG->_("Makes 'df' act like on BSD")?>
                   </td>
                   <td><br><input type="radio" name="statfs" value="minixdf"<?php if($_REQUEST["statfs"] == 'minixdf') { echo " CHECKED"; } ?>><?=$LANG->_("Makes 'df' act like on Minix")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('What group id will a newly created file gets')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="groupid" value="bsdgroups"<?php if($_REQUEST["groupid"] == 'bsdgroups') { echo " CHECKED"; } ?>><?=$LANG->_("Give objects the same group ID as their parent")?>
                   </td>
                   <td><br><input type="radio" name="groupid" value="sysvgroups"<?php if($_REQUEST["groupid"] == 'sysvgroups') { echo " CHECKED"; } ?>><?=$LANG->_("New objects have the group ID of their creator")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Allocator to use for new inode')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="allocator" value="oldalloc"<?php if($_REQUEST["allocator"] == 'oldalloc') { echo " CHECKED"; } ?>><?=$LANG->_("Old allocator")?>
                   </td>
                   <td><br><input type="radio" name="allocator" value="orlov"<?php if($_REQUEST["allocator"] == 'orlov') { echo " CHECKED"; } ?>><?=$LANG->_("Orlov allocator")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Define the behaviour when an error is encountered')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="error" value="continue"<?php if($_REQUEST["error"] == 'continue') { echo " CHECKED"; } ?>><?=$LANG->_('Keep going on a filesystem error')?>
                   </td>
                   <td><input type="radio" name="error" value="remount"<?php if($_REQUEST["error"] == 'remount') { echo " CHECKED"; } ?>><?=$LANG->_('Remount the filesystem read-only on an error')?><br>
                       <input type="radio" name="error" value="panic"<?php if($_REQUEST["error"] == 'panic') { echo " CHECKED"; } ?>><?=$LANG->_('Panic and halt the machine if an error occurs')?>
                   </td>
                 </tr>
<?php
// }}}

		} elseif(($_REQUEST["filesystem"] == 'msdos') or ($_REQUEST["filesystem"] == 'umsdos') or ($_REQUEST["filesystem"] == 'vfat')) {
		  // {{{ Mount options for msdos, umsdos and/or vfat
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="fat_debug"<?php if($_REQUEST["fat_debug"]) { echo " CHECKED"; } ?>><?=$LANG->_('Print debugging info upon each (re)mount')?></td>
                   <td><input type="checkbox" name="fat_quiet"<?php if($_REQUEST["fat_quiet"]) { echo " CHECKED"; } ?>><?=$LANG->_('Attempts to chown or chmod files do not return errors, although they fail')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Text format conversion')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="textconv" value="binary"<?php if($_REQUEST["textconv"] == 'binary') { echo " CHECKED"; } ?>><?=$LANG->_('No translation')?>
                   </td>
                   <td>
                     <input type="radio" name="textconv" value="text"<?php if($_REQUEST["textconv"] == 'text') { echo " CHECKED"; } ?>><?=$LANG->_('CRLF<-->NL translation')?><br>
                     <input type="radio" name="textconv" value="auto"<?php if($_REQUEST["textconv"] == 'auto') { echo " CHECKED"; } ?>><?=$LANG->_('CRLF<-->NL translation on all files not having "well-known binary" extension')?>
                   </td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Check pickyness')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="check" value="relaxed"<?php if($_REQUEST["check"] == 'relaxed') { echo " CHECKED"; } ?>><?=$LANG->_('Relaxed')?>
                   </td>
                   <td><input type="radio" name="check" value="normal"<?php if($_REQUEST["check"] == 'normal') { echo " CHECKED"; } ?>><?=$LANG->_('Normal')?><br>
                       <input type="radio" name="check" value="strict"<?php if($_REQUEST["check"] == 'strict') { echo " CHECKED"; } ?>><?=$LANG->_('Strict')?>
                   </td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('FAT Type')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="fattype" value="12"<?php if($_REQUEST["fattype"] == '12') { echo " CHECKED"; } ?>>12
                   </td>
                   <td><input type="radio" name="fattype" value="16"<?php if($_REQUEST["fattype"] == '16') { echo " CHECKED"; } ?>>16
                       <input type="radio" name="fattype" value="32"<?php if($_REQUEST["fattype"] == '32') { echo " CHECKED"; } ?>>32
                   </td>
                 </tr>
<?php
// }}}

		} elseif($_REQUEST["filesystem"] == 'ntfs') {
		  // {{{ Mount options for NTFS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="utf8"<?php if($_REQUEST["utf8"]) { echo " CHECKED"; } ?>><?=$LANG->_('Use UTF-8 for converting file names')?></td>
                   <td><input type="checkbox" name="posix"<?php if($_REQUEST["posix"]) { echo " CHECKED"; } ?>><?=$LANG->_('Distinguishes between upper and lower case')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Use escape sequences for unknown Unicode characters')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="uni_xlate" value="1"<?php if($_REQUEST["uni_xlate"] == '1') { echo " CHECKED"; } ?>><?=$LANG->_('True')?>
                   </td>
                   <td>
                     <input type="radio" name="uni_xlate" value="2"<?php if($_REQUEST["uni_xlate"] == '2') { echo " CHECKED"; } ?>><?=$LANG->_('Use VFAT style 4byte escape sequences')?><br>
                     <input type="radio" name="uni_xlate" value="0"<?php if($_REQUEST["uni_xlate"] == '0') { echo " CHECKED"; } ?>><?=$LANG->_('False')?>
                   </td>
                 </tr>
<?php
// }}}

		} elseif($_REQUEST["filesystem"] == 'reiserfs') {
		  // {{{ Mount options for reiserfs
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_conv"<?php if($_REQUEST["reiser_conv"]) { echo " CHECKED"; } ?>><?=$LANG->_('Instructs version 3.6 reiserfs software to mount a version 3.5 FS')?></td>
                   <td><input type="checkbox" name="reiser_hashreloc"<?php if($_REQUEST["reiser_hashreloc"]) { echo " CHECKED"; } ?>><?=$LANG->_('Tunes the block allocator')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_nounhashreloc"<?php if($_REQUEST["reiser_nounhashreloc"]) { echo " CHECKED"; } ?>><?=$LANG->_('Tunes the block allocator')?></td>
                   <td><input type="checkbox" name="reiser_noborder"<?php if($_REQUEST["reiser_noborder"]) { echo " CHECKED"; } ?>><?=$LANG->_('Disable the border allocator algorithm')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_nolog"<?php if($_REQUEST["reiser_nolog"]) { echo " CHECKED"; } ?>><?=$LANG->_('Disable journalling')?></td>
                   <td><input type="checkbox" name="reiser_notail"<?php if($_REQUEST["reiser_notail"]) { echo " CHECKED"; } ?>><?=$LANG->_("Disable packing small files and 'file tails' in the tree")?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="reiser_replayonly"<?php if($_REQUEST["reiser_replayonly"]) { echo " CHECKED"; } ?>><?=$LANG->_('Replay the transactions which are in the journal, but do actually mount the file system')?></td>
                   <td></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Hash function to use for finding files in directories')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="hash" value="rupasov"<?php if($_REQUEST["hash"] == 'rupasov') { echo " CHECKED"; } ?>>rupasov&nbsp;&nbsp;&nbsp;
                     <input type="radio" name="hash" value="tea"<?php if($_REQUEST["hash"] == 'tea') { echo " CHECKED"; } ?>>tea
                   </td>
                   <td>
                       <input type="radio" name="hash" value="r5"<?php if($_REQUEST["hash"] == 'r5') { echo " CHECKED"; } ?>>r5<br>
                       <input type="radio" name="hash" value="detect"<?php if($_REQUEST["hash"] == 'detect') { echo " CHECKED"; } ?>>detect
                   </td>
                 </tr>
<?php
// }}}

		} elseif($_REQUEST["filesystem"] == 'xfs') {
		  // {{{ Mount options for XFS
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="dmapi"<?php if($_REQUEST["dmapi"]) { echo " CHECKED"; } ?>><?=$LANG->_('Enable the DMAPI event callouts')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="noalign"<?php if($_REQUEST["noalign"]) { echo " CHECKED"; } ?>><?=$LANG->_('Do not aligned data allocations at stripe unit boundaries')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="noatime"<?php if($_REQUEST["noatime"]) { echo " CHECKED"; } ?>><?=$LANG->_('Do not update access timestamps')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="norecovery"<?php if($_REQUEST["norecovery"]) { echo " CHECKED"; } ?>><?=$LANG->_('Mount filesystem without running log recovery')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="nouuid"<?php if($_REQUEST["nouuid"]) { echo " CHECKED"; } ?>><?=$LANG->_('Ignore the filesystem uuid')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="osyncisdsync"<?php if($_REQUEST["osyncisdsync"]) { echo " CHECKED"; } ?>><?=$LANG->_('Make writes to files opened with the O_SYNC flag set behave\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;as if the O_DSYNC flag had been used instead.')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="usrquota" onChange="submit()"<?php if($_REQUEST["usrquota"]) { echo " CHECKED"; } ?>><?=$LANG->_('Enable user quota')?>&nbsp;&nbsp;</td>
<?php	if(@$_REQUEST["usrquota"]) { ?>
                   <td><input type="checkbox" name="uqnoenforce" onChange="submit()"<?php if($_REQUEST["uqnoenforce"]) { echo " CHECKED"; } ?>><?=$LANG->_('Enforce user quota limits')?></td>
<?php	} ?>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="grpquota" onChange="submit()"<?php if($_REQUEST["grpquota"]) { echo " CHECKED"; } ?>><?=$LANG->_('Enable group quota')?>&nbsp;&nbsp;</td>
<?php	if(@$_REQUEST["grpquota"]) { ?>
                   <td><input type="checkbox" name="gqnoenforce" onChange="submit()"<?php if($_REQUEST["gqnoenforce"]) { echo " CHECKED"; } ?>><?=$LANG->_('Enforce group quota limits')?></td>
<?php	} ?>
                 </tr>
<?php
// }}}

		} elseif($_REQUEST["filesystem"] == 'iso9660') {
		  // {{{ Mount options for ISO9660
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="norock" onChange="submit()"<?php if($_REQUEST["norock"]) { echo " CHECKED"; } ?>><?=$LANG->_('Disable the use of Rock Ridge extensions')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="nojoliet" onChange="submit()"<?php if($_REQUEST["nojoliet"]) { echo " CHECKED"; } ?>><?=$LANG->_('Disable the use of Microsoft Joliet extensions')?></td>
                 </tr>

<?php	if(@$_REQUEST["norock"]) { ?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td>
                     <b><?=$LANG->_('Type of filename lookup')?>:</b>&nbsp;&nbsp;<br>
                     <input type="radio" name="check" value="relaxed"<?php if($_REQUEST["check"] == 'relaxed') { echo " CHECKED"; } ?>><?=$LANG->_('Relaxed')?>&nbsp;&nbsp;&nbsp;
                     <input type="radio" name="check" value="strict"<?php if($_REQUEST["check"] == 'strict') { echo " CHECKED"; } ?>><?=$LANG->_('Strict')?>
                   </td>
                   <td></td>
                 </tr>
<?php	} ?>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="unhide"<?php if($_REQUEST["unhide"]) { echo " CHECKED"; } ?>><?=$LANG->_('Show hidden and associated files')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="cruft"<?php if($_REQUEST["cruft"]) { echo " CHECKED"; } ?>><?=$LANG->_('Ignore the high order bits of the file length')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="utf8"<?php if($_REQUEST["utf8"]) { echo " CHECKED"; } ?>><?=$LANG->_('Use UTF-8 for converting file names')?></td>
                   <td></td>
                 </tr>

<?php
// }}}

		} elseif($_REQUEST["filesystem"] == 'udf') {
		  // {{{ Mount options for udf
?>
                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="unhide"<?php if($_REQUEST["unhide"]) { echo " CHECKED"; } ?>><?=$LANG->_('Show otherwise hidden files')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="undelete"<?php if($_REQUEST["undelete"]) { echo " CHECKED"; } ?>><?=$LANG->_('Show deleted files in lists')?></td>
                 </tr>

                 <tr class="<?php pql_format_table(); ?>">
                   <td><input type="checkbox" name="nostrict"<?php if($_REQUEST["nostrict"]) { echo " CHECKED"; } ?>><?=$LANG->_('Unset strict conformance')?>&nbsp;&nbsp;</td>
                   <td><input type="checkbox" name="novrs"<?php if($_REQUEST["novrs"]) { echo " CHECKED"; } ?>><?=$LANG->_('Skip volume sequence recognition')?>&nbsp;&nbsp;</td>
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

                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Some day, I''ll add radio buttons for<br>
                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;you to click here...<br>

                           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Please separate your options with comma.
                   </td>
                 </tr>
<?php
// }}}
		}
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
    <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
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

	// {{{ Setup DN
	if(($type == 'add') or ($type == 'modify')) {
	  if(@$_REQUEST["mountpoint"] and ($_REQUEST["basemount"] == 'new')) {
		// Prefix the DN with the name of the mounpoint
		$auto_master = attribute_find_automaster($_REQUEST["host"]);
		if(!$_pql->get_dn($auto_master, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*')) {
		  // First ever automount for this host - create the auto.master object
		  // Can't use pql_unit_add() here because it creates an organizationalUnit
		  // object, and we want a automountMap...
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'automountMap';
		  $entry["ou"] = "auto.master";
		  
		  if(!$_pql->add($auto_master, $entry, 'unit', 'attrib.automount.inc:attribute_save_add'))
			die("Could not add auto.master object!");
		}
		
		$dn = $auto_master;
	  } else {
		$info = $_pql->get_attribute($_REQUEST["basemount"], pql_get_define("PQL_ATTR_AUTOMOUNT_INFO"));
		if(eregi('ldap', $info)) {
		  // An LDAP link
		  $dn = split(':', $info);
		  if($dn[2])
			// info => 'ldap ldapi://%2fvar%2frun%2fslapd%2fldapi.provider:ou=auto.mnt,cn=pumba.bayour.com,ou=Computers,c=SE'
			$dn = $dn[2];
		  elseif($dn[1])
			// info => 'ldap localhost:ou=auto.share,cn=pumba.bayour.com,ou=Computers,c=SE'
			$dn = $dn[1];
		} elseif(eregi('file ', $info)) {
		  // A file link - can't follow that!
?>
Sorry, this base mount is a <b>file</b> link and I don''t know how to follow that...
<?php
		} else {
		  // Don't know what to do now... Die!
		  $object = $_pql->search($dn, 'objectClass=*', BASE);
?>
<font color="red">Sorry, this base mount is not an LDAP link nor is it a file link. Don't know what to do now...</font><p>
Please report this to either to the <a href="http://bugs.bayour.com/" target="_new">bugtracker</a>
or to the <a href="mailto:phpqladmin@lists.bayour.com">mailinglist</a><br>
Make sure you include this object information (I'm mostly interested in the <b><?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?></b> attribute):
<?php
		  printr($object);
		  die();
		}
	  }

	  // Prefix the DN with the name of the mounpoint
	  $dn = pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["mountpoint"].",$dn";
	}
// }}}

    switch($type) {
      case "add":
		// {{{ Add a automount
		// {{{ Setup object
		$object["dn"] = $dn;
		$object[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'autoMount';
		if(@$_REQUEST["description"])
			$object[pql_get_define("PQL_ATTR_DESCRIPTION")] = $_REQUEST["description"];
		if(@$_REQUEST["mountpoint"])
			$object[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["mountpoint"];

		$object[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = attribute_setup();
// }}}

		// {{{ Retreive parent ACI attribute(s)
		// Calculate parent object
		$dn_parts = split(',', $dn);
		unset($dn); // Reuse
		for($i=1; $dn_parts[$i]; $i++) {
		  $dn .= $dn_parts[$i];
		  if($dn_parts[$i+1])
			$dn .= ",";
		}

		// Get the ACI's of this object
		$object[pql_get_define("PQL_ATTR_LDAPACI")] = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_LDAPACI"), 1);
		// }}}

		// {{{ Add object to LDAP server
		// The ':_add' is only so that pql_create_ldif() will not put the 'replace: ...' lines in the LDIF.
		if($_pql->add($object["dn"], $object, 'automount', 'attrib.automount.inc:attribute_save_add')) {
		  if(@$_REQUEST["mountpoint"] and (($_REQUEST["basemount"] == 'new') or ($_REQUEST["filesystem"] == 'autofs'))) {
			// Also create the 'auto.<mountpoint>' object
			$mnt  = pql_get_define("PQL_ATTR_OU").'=auto.'.preg_replace('/\//', '', $_REQUEST["mountpoint"]);
			$mnt .= ','.$_REQUEST["host"];

			$dnparts = split(',', $mnt);
			$parentdn = '';
			for($i=1; $dnparts[$i]; $i++) {
			  $parentdn .= $dnparts[$i];
			  if($dnparts[$i+1])
				$parentdn .= ',';
			}

			if(pql_unit_add($parentdn, $dnparts[0]))
			  $msg = '<br>'.pql_complete_constant($LANG->_('Successfully added %what%'),
												  array('what' => $LANG->_('automountpoint')));
			else
			  $msg = '<br>'.pql_complete_constant($LANG->_('Failed to add %what%'),
												  array('what' => $LANG->_('automountpoint')));
		  }

		  $msg = '<br>'.pql_complete_constant($LANG->_('Successfully added %what%'),
											  array('what' => $LANG->_('automountpoint')));
		} else
		  $msg = $LANG->_('Failed to add automount').": ".ldap_error($_pql->ldap_linkid);
// }}}

		// Redirect to correct domain view page
		attribute_forward($msg);
		break;
// }}}

      case "modify":
		// {{{ Modify a automount
		if(!$_pql->get_dn($dn, pql_get_define("PQL_ATTR_OBJECTCLASS".'=*'), 'BASE')) {
		  // We probably 'changed' the location by selecting another base mountpoint or
		  // by selecting another mount point. This means that the new location doesn't
		  // exist, so ADD a new object to the database...

		  // {{{ Setup object
		  $object["dn"] = $dn;
		  $object[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'autoMount';
		  if(@$_REQUEST["description"])
			$object[pql_get_define("PQL_ATTR_DESCRIPTION")] = $_REQUEST["description"];
		  if(@$_REQUEST["mountpoint"])
			$object[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["mountpoint"];
		  
		  $object[pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = attribute_setup();
		  // }}}
		  
		  // Add new object to database
		  if($_pql->add($dn, $object, 'automount', 'attrib.automount.inc:attribute_save_add'))
			// TODO: Should we remove the old? I don't think so - user might want duplicates.
			//       For example: I have '/mnt/cdrom1' and '/mnt/cdrom/1' with the same options
			//       etc... The first is for convenience (and because I don't always get the
			//       submount - the second one - correct :).
			$msg = $LANG->_('Successfully added automount');
		  else
			$msg = $LANG->_('Failed to add automount').": ".ldap_error($_pql->ldap_linkid);
		} else { 
		  // Setup (new) automountInformation attribute
		  $automount = attribute_setup();

		  // Do modification
		  if(pql_modify_attribute($dn, pql_get_define("PQL_ATTR_AUTOMOUNT_INFO"), 1, $automount))
			$msg = $LANG->_('Successfully modified automount');
		  else
			$msg = $LANG->_('Failed to modify automount').": ".ldap_error($_pql->ldap_linkid);
		}

		attribute_forward($msg);
		break;
// }}}

      case "delete":
		// {{{ Delete the automount information in $_REQUEST["mount"]
		if($_pql->delete($_REQUEST["mount"]))
		  $msg = pql_complete_constant($LANG->_('Successfully removed mountpoint %mount%'),
									   array('mount' => $_REQUEST["mountpoint"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove mountpoint %mount%'),
									   array('mount' => $_REQUEST["mountpoint"])) . ": " . ldap_error($_pql->ldap_linkid);
		
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
