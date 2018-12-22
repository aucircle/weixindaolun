const deviceInfoURL = 'https://api.heclouds.com/devices/505360315';
const getDataStreamURL = 'https://api.heclouds.com/devices/505360315/datastreams';
const sendCommandURL = 'https://api.heclouds.com/cmds';




Page(
  {
    data: { isChecked: false},
    onLoad: function () {
      var timer = setInterval( () => {
        getDeviceInfo(this)
      }, 3000)
    },
   /*calm:{},
   warm:{},
   up:{},
   down:{},*/

control: function(){
  controlLED()
}


  })

  
function getDeviceInfo(that) {

  var deviceConnected;
  //查看设备连接状态，并刷新按钮状态
  wx.request({
    url: 'https://api.heclouds.com/devices/509175897',
    header: {
      // 'content-type': 'application/x-www-form-urlencoded',
      "api-key": 'gfRSTUAQrJSqxRmD7gy=F=pu=Ic='
    },
    method: "GET",
    success(res) {
      console.log(res)
      if (res.data.data.online) {
        console.log("设备已经连接")
        deviceInit(that)//初始化按钮
        deviceConnected = true
      } else {
        console.log("设备未连接")
        deviceConnected = false
      }
    },
    fail(res) {
      console.log("请求失败", res)
      deviceConnected = false
    },
    complete() {
      if (deviceConnected) {
        that.setData({ deviceConnected: true })
      } else {
        that.setData({ deviceConnected: false })
      }
    }
  })
}



function deviceInit(that) {
  console.log("开始初始化按钮")
  //初始化各个硬件的状态
  wx.request({
    url: 'https://api.heclouds.com/devices/505360315/datastreams',
    header: {
      'content-type': 'application/x-www-form-urlencoded',
      "api-key": 'BI0MJ53xsG6vcYsR9ILmX3AcEvU ='
    },
    data: {

    },
    success(res) {
      console.log(res)
      for (var i = 0; i < res.data.data.length; i++) {
        var info = res.data.data[i]

        switch (info.id) {
          case "Blue_Led":
            if (info.current_value == 1) {
              that.setData({ blue_checked: true })
            } else {
              that.setData({ blue_checked: false })
            }
            break
          case "Beep":
            if (info.current_value == 1) {
              that.setData({ beep_checked: true })
            } else {
              that.setData({ beep_checked: false })
            }
            break
          case "Red_Led":
            if (info.current_value == 1) {
              that.setData({ red_checked: true })
            } else {
              that.setData({ red_checked: false })
            }
            break
          case "Tempreture":
            break
          case "Yellow_Led":
            if (info.current_value == 1) {
              that.setData({ yellow_checked: true })
            } else {
              that.setData({ yellow_checked: false })
            }
            break
          case "Green_Led":
            if (info.current_value == 1) {
              that.setData({ green_checked: true })
            } else {
              that.setData({ green_checked: false })
            }
            break
          case "Humidity":
            break
        }
      }
    }
  })
}

function controlLED(hardware_id, switch_value) {

  //console.log("发送命令：" + hardware_id + ":" + switch_value)
  //按钮发送命令控制硬件
  console.log("on/off" + 509175897 + ":" + on)
  wx.request({
    url: 'https://api.heclouds.com/cmds?device_id=509175897',
    method: 'POST',
    header: {
      'content-type': 'application/x-www-form-urlencoded',
      "api-key": 'gfRSTUAQrJSqxRmD7gy=F=pu=Ic='
    },

    data: hardware_id + ":" + switch_value,
    success(res) {
      console.log("控制成功")
      console.log(res)
    }
  })
}


