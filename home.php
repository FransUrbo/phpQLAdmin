<?php
// start page
// $Id: home.php,v 1.2 2002-12-12 11:54:15 turbo Exp $
//
session_start();
require("pql.inc");

include("header.html");
?>
  <!-- User base: <?php echo $USER_BASE; ?> -->
<?php
// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and PQL_AUTO_RELOAD){
?>
  <script language="JavaScript1.2">
  <!--
  // reload navigation frame
     parent.frames.pqlnav.location.reload();
  //-->
  </script>
<?php
}
?>

  <br><?php echo PQL_DESCRIPTION ?><br>

  <ul>
    <li>
      <form action="domain_add.php" method="post">
        <?php echo PQL_DOMAIN_ADD; ?>
        <br>
        <input type="text" name="domain" value="<?php echo $domain; ?>">
        <input type="submit" value="<?php echo PQL_ADD; ?>">
       </form>
       <br>
    </li>

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

    <li><a href="doc/index.php"><?php echo PQL_DOCUMENTATION; ?></a></li>
    <li><a href="config_detail.php"><?php echo PQL_SHOW_CONFIG; ?></a></li>
    <li><a href="config_ldaptest.php"><?php echo PQL_TEST_LDAP; ?></a></li>
    <li><a href="http://www.adfinis.ch/projects/phpQLAdmin/" target="_new">phpQLAdmin Online</a></li>
    <li><a href="http://www.adfinis.ch/" target="_new">adfinis GmbH, the developpers</a></li>
  </ul>
<br>
&copy; <a href="http://www.adfinis.ch">adfinis GmbH</a>, 2001 (<a href="mailto:info@adfinis.ch">info</a>)
</body>
</html>
