//! Rust cdev_sample sample.

use kernel::prelude::*;

module! {
    type: CDevSample,
    name: "cdev_sample",
    author: "John Doe",
    description: "cdev sample",
    license: "GPL",
}

#[link(name = "helper_func")]
extern "C" {
    #[allow(dead_code)]
    fn helper_func();
}

#[link(name = "init_cdev")]
extern "C" {
    fn init_cdev() -> core::ffi::c_int;
}

#[link(name = "cleanup_cdev")]
extern "C" {
    fn cleanup_cdev();
}

struct CDevSample;

impl kernel::Module for CDevSample {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("Rust cdev_sample sample (init)\n");
        pr_info!("Am I built-in? {}\n", !cfg!(MODULE));

        let ret = unsafe { init_cdev() };
        if ret != 0 {
            pr_err!("ret: {}\n", ret);
            Err(kernel::error::to_result(ret).err().unwrap())
        } else {
            Ok(CDevSample)
        }
    }
}

impl Drop for CDevSample {
    fn drop(&mut self) {
        unsafe { cleanup_cdev() };
        pr_info!("Rust cdev_sample sample (exit)\n");
    }
}
