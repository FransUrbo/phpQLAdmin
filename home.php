<?php
// start page
// home.php,v 1.3 2002/12/13 13:50:12 turbo Exp
//
session_start();

require("pql.inc");

include("header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// find out which LDAP server(s) to use
if($change_ldap_server_users or $change_ldap_server_controls) {
    // Change LDAP server for user database?
    if($ldapserver) {
	$host = split(';', $ldapserver);
	$USER_HOST_USR = $host[0] . ";" . $host[1];
	$USER_SEARCH_DN_USR = $host[2];

	session_register("USER_HOST_USR", "USER_SEARCH_DN_USR");
    }

    // Change LDAP server for controls database?
    if($controlserver) {
	$host = split(';', $ldapserver);
	$USER_HOST_CTR = $host[0] . ";" . $host[1];
	$USER_SEARCH_DN_CTR = $host[2];
	session_register("USER_HOST_CTR", "USER_SEARCH_DN_CTR");
    }
}
?>

  <br><span class="title1"><?php echo PQL_DESCRIPTION ?></span><br>

  <ul>
<?php
	// TODO: How do we know if the user is allowed to add domains?
	//       In the domain description we don't have that info...
	// if(($ADVANCED_MODE == 1) && ($USER_BASE == 'everything')){
	if($ADVANCED_MODE == 1) {
	    // We're administrating the whole domain,
	    // show the Create domain option...

	    if(eregi(" ", PQL_LDAP_HOST)) {
		$servers_usr = split(" ", PQL_LDAP_HOST);
?>
    <li>
      <form action="<?=$PHP_SELF?>" target="pqlmain">
	Change LDAP server (user database)<br>
        <select name="ldapserver">
<?php
		foreach($servers_usr as $server) {
?>
          <option value="<?=$server?>"><?=$server?></option>
<?php
		}
	    }
?>
        </select>
        <input type="hidden" name="change_ldap_server_users" value=1>
        <input type="submit" value="<?="--&gt;&gt;"?>" onChange="refreshFrames();">
      </form>
    </li>

<?php
	    if(PQL_LDAP_CONTROL_USE and eregi(" ", PQL_LDAP_CONTROL_HOST)) {
		$servers_ctr = split(" ", PQL_LDAP_CONTROL_HOST);
?>
    <li>
      <form action="<?=$PHP_SELF?>" target="pqlmain">
        Change LDAP server (controls database)<br>
        <select name="controlserver">
<?php
                foreach($servers_ctr as $server) {
?>
          <option value="<?=$server?>"><?=$server?></option>
<?php
                }
?>
        </select>
        <input type="hidden" name="change_ldap_server_controls" value=1>
        <input type="submit" value="<?="--&gt;&gt;"?>" onChange="refreshFrames();">
      </form>
    </li>
<?php
	    }
?>


    <li>
      <form action="domain_add.php" method="post">
	<?=PQL_DOMAIN_ADD?>
        <br>
        <input type="text" name="domain" value="<?php echo $domain; ?>">
        <input type="submit" value="<?="--&gt;&gt;"?>">
	<br>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td>
                <img src="images/info.png" width="16" height="16" alt="" border="0"></td>
              <td>
                <?php
	    if(PQL_LDAP_OBJECTCLASS_DOMAIN == "domain") {
		// We're using a domain object
		echo PQL_DOMAIN_ADD_INFO_DC;
	    } else {
		// OrganizationUnit object
		echo PQL_DOMAIN_ADD_INFO_OU;
	        }?>
              </td>
            </tr>
          </th>
        </table>
      </form>
      <br>
    </li>
<?php
	}
?>
    <!-- begin search engine snippet -->
    <li>
      <form method=post action="search.php">
        <?php echo PQL_SEARCH_TITLE; ?>
        <br>
        <select name=attribute>
          <option value=<?php echo PQL_LDAP_ATTR_UID; ?> SELECTED><?php echo PQL_SEARCH_UID; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_CN; ?>><?php echo PQL_SEARCH_CN; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_MAIL; ?>><?php echo PQL_SEARCH_MAIL; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_MAILALTERNATE; ?>><?php echo PQL_SEARCH_MAILALTERNATEADDRESS; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_FORWARDS; ?>><?php echo PQL_SEARCH_MAILFORWARDINGADDRESS; ?></option>
        </select>

        <select name=filter_type>
       	  <option value=contains SELECTED><?php echo PQL_SEARCH_CONTAINS; ?></option>
          <option value=is><?php echo PQL_SEARCH_IS; ?></option>
          <option value=starts_with><?php echo PQL_SEARCH_STARTSWITH; ?></option>
          <option value=ends_with><?php echo PQL_SEARCH_ENDSWITH; ?></option>
        </select>

        <input type=text name=search_string length=20>
        <input type=submit value="<?php echo PQL_SEARCH_FINDBUTTON; ?>">
      </form>
    </li>
    <!-- end of search engine snippet -->
  </ul>
  <br>

<?php include("trailer.html"); ?>
</body>
</html>
