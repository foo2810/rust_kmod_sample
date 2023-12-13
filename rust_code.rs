//! Rust mix_rust_c sample.

use kernel::prelude::*;

module! {
    type: RustMinimal,
    name: "mix_rust_c",
    author: "John Doe",
    description: "Mix rut and c codes",
    license: "GPL",
}

#[link(name = "helper_func")]
extern "C" {
    fn helper_func();
}

struct RustMinimal;

impl kernel::Module for RustMinimal {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("Rust mix_rust_c sample (init)\n");
        pr_info!("Am I built-in? {}\n", !cfg!(MODULE));

        unsafe { helper_func() };

        Ok(RustMinimal)
    }
}

impl Drop for RustMinimal {
    fn drop(&mut self) {
        pr_info!("Rust mix_rust_c sample (exit)\n");
    }
}
