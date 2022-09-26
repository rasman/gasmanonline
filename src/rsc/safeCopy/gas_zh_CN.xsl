<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg">
  <xsl:template match="/">
    <html>
      <head>

        <style type="text/css">
          body {text-align: left; font-family: Microsoft YaHei, Verdana, Geneva, Arial, Helvetica, sans-serif; font-size: 12px;}
          h2 {font-family: "Microsoft YaHei, Arial"; font-size: 25px;}
          h3 {font-family: "Microsoft YaHei, Century Gothic"; font-size: 12px; text-align: left;}
          table {font-family: Microsoft YaHei, Verdana, Geneva, Arial, Helvetica, sans-serif; font-size: 11px;}
          .header_three {font-family: "Microsoft YaHei, Trebuchet MS"; font-size: 12px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid #096361; background-color: #8A9374; font-style: normal; color: #FFFFFF; font-weight: bold;}
          .header_four {font-family: "Microsoft YaHei, Trebuchet MS"; font-size: 11px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid #CC0000; background-color: #FFDFDF;}
          .header_one {font-family: "Microsoft YaHei, Trebuchet MS"; font-size: 12px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid #065351; background-color: #B7E1DF; font-weight: bold;}
          .header_two {font-family: "Microsoft YaHei, Trebuchet MS"; font-size: 12px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid Gray; background-color: #F1F1F1; font-weight: bold;}
          .line {text-align: center;}
          .line_two {background-color: #F5F5F5; text-align: center;}
        </style>

      </head>
      <body>
        <table width="350" border="0" align="center">
          <tr>
            <td width="150" align="center">
              <img src="{gasman/@iconPath}" width="128"/>
            </td>
            <td align="left" valign="middle">
              <h2>
                Gas Man<xsl:text disable-output-escaping="yes">&amp;reg;</xsl:text> 模拟
              </h2>
            </td>
          </tr>
        </table>

        <table width="50%" border="0" class="header_two" align="center">
          <tr>
            <td width="20%" align="left">日期/时间：</td>
            <td width="80%" align="left">
              <xsl:value-of  select="gasman/@datetime"/>
            </td>
          </tr>
          <tr>
            <td width="20%" align="left">说明：</td>
            <td width="80%" align="left">
              <xsl:value-of  select="gasman/@description"/>
            </td>
          </tr>
        </table>
        <table width="95%"  border="0" align="center" cellpadding="0" cellspacing="0">
          <tr>
            <td colspan="2">

              <hr/>
            </td>
          </tr>
          <tr>
            <td width="60%">
              <table width="100%" border="0" align="left">
                <tr>
                  <td height="20" colspan="3" class="header_one" align="center">患者参数</td>
                  <td height="20" align="center" colspan="2" class="header_one">血流</td>
                </tr>
                <tr>
                  <td  >体重</td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/weight/@value"/>
                  </td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/weight/@unit"/>
                  </td>
                  <td>富血管</td>
                  <td>
                    <xsl:variable name="vrgpct" select="gasman/params/patient/flows/@vrg"/>
                    <xsl:value-of select='format-number($vrgpct, "##%")' />
                  </td>
                </tr>
                <tr>
                  <td  >脂肪</td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/volumes/@fat"/>
                  </td>
                  <td>L</td>
                  <td>肌肉</td>
                  <td>
                    <xsl:variable name="muspct" select="gasman/params/patient/flows/@mus"/>
                    <xsl:value-of select='format-number($muspct, "##%")' />
                  </td>
                </tr>

                <tr>
                  <td  >肺泡</td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/volumes/@alv"/>
                  </td>
                  <td>L</td>
                  <td>脂肪</td>
                  <td>
                    <xsl:variable name="fatpct" select="gasman/params/patient/flows/@fat"/>
                    <xsl:value-of select='format-number($fatpct, "##%")' />
                  </td>
                </tr>

                <tr>
                  <td  >肌肉</td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/volumes/@mus"/>
                  </td>
                  <td>L</td>
                  <td> </td>
                  <td> </td>
                </tr>
                <tr>
                  <td  >富血管</td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/volumes/@vrg"/>
                  </td>
                  <td>L</td>
                  <td> </td>
                  <td> </td>
                </tr>
                <tr>
                  <td  >静脉</td>
                  <td>
                    <xsl:value-of select="gasman/params/patient/volumes/@ven"/>
                  </td>
                  <td>L</td>
                  <td> </td>
                  <td> </td>
                </tr>
              </table>
            </td>
            <td width="40%" align="left" valign="top">
              <table width="100%"  border="0" align="right">
                <tr>
                  <td height="20" colspan="5" class="header_one" align="center">麻醉剂</td>
                </tr>
                <tr>
                  <td align="center" class="header_two">名称</td>
                  <td colspan="3" align="center" class="header_two">可溶性</td>
                  <td align="center" class="header_two">挥发性</td>
                </tr>
                <tr>
                  <td > </td>
                  <td  align="center" class="header_two">血液</td>
                  <td  align="center" class="header_two">血流丰富组织</td>
                  <td  align="center" class="header_two">脂肪</td>
                  <td  align="center"> </td>
                </tr>
                <xsl:for-each select="gasman/params/agents/agent">
                  <tr>
                    <td >
                      <xsl:value-of select="@name"/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaBlood, "#0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaVrg, "#0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaFat, "##0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select="@volatility"/>
                    </td>
                  </tr>
                </xsl:for-each>
              </table>
            </td>
          </tr>
        </table>
        <hr width="95%"/>
        <xsl:variable name="shouldPrintPrintout">
          <xsl:value-of select="gasman/@printPrintout" />
        </xsl:variable>
        <xsl:if test="$shouldPrintPrintout != 'false'">
          <table width="95%"  border="0" align="center">
            <tr>
              <td class="header_three" align="center">时间</td>
              <td class="header_three" align="center">吸入麻醉剂</td>
              <td class="header_three" align="center">挥发罐 (%atm)</td>
              <td class="header_three" align="center">已注射的</td>
              <td class="header_three" align="center">环路系统</td>
              <td class="header_three" align="center">新鲜气流 (L/m)</td>
              <td class="header_three" align="center">分钟通气量 (L/m)</td>
              <td class="header_three" align="center">心输出量 (L/m)</td>
            </tr>
            <xsl:for-each select="gasman/settings/setting">
              <xsl:if test="count(agentsettings/agentsetting) = 1">
                <tr class="line_two">
                  <td align="center">
                    <xsl:value-of select="@time"/>
                  </td>
                  <td align="center" class="line">
                    <xsl:value-of select="agentsettings/agentsetting[1]/@agentname"/>
                  </td>
                  <td align="center" class="line">
                    <xsl:value-of select='format-number(agentsettings/agentsetting[1]/@del, "##0.##")'/>
                  </td>
                  <td align="center" class="line">
                    <xsl:value-of select='format-number(agentsettings/agentsetting[1]/@inject, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select="@circuit"/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@fgf, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@va, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@co, "##0.##")'/>
                  </td>
                </tr>
              </xsl:if>
              <xsl:if test="count(agentsettings/agentsetting) != 1">
                <tr class="line_two">
                  <td align="center">
                    <xsl:value-of select="@time"/>
                  </td>
                  <td align="center"/>
                  <td align="center"/>
                  <td align="center"/>
                  <td align="center">
                    <xsl:value-of select="@circuit"/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@fgf, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@va, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@co, "##0.##")'/>
                  </td>
                </tr>
                <xsl:for-each select="agentsettings/agentsetting">
                  <tr>
                    <td align="center" class="line"/>
                    <td align="center" class="line">
                      <xsl:value-of select="@agentname"/>
                    </td>
                    <td align="center" class="line">
                      <xsl:value-of select='format-number(@del, "##0.##")'/>
                    </td>
                    <td align="center" class="line">
                      <xsl:value-of select='format-number(@inject, "##0.##")'/>
                    </td>
                  </tr>
                </xsl:for-each>
              </xsl:if>
            </xsl:for-each>
          </table>
        </xsl:if>
        <xsl:variable name="shouldPrintGraph">
          <xsl:value-of select="gasman/@printGraph" />
        </xsl:variable>
        <xsl:if test="$shouldPrintGraph != 'false'">
          <div align="center">
            <img>
              <xsl:attribute name="src">
                <xsl:value-of select="gasman/@externalImage" />
              </xsl:attribute>
            </img>
          </div>
        </xsl:if>
      </body>
    </html>

  </xsl:template>
</xsl:stylesheet>
