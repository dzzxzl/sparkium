#pragma once

namespace sparks {
struct RendererSettings {
  int num_samples{1};
  // int num_bounces{16};
  int num_bounces{1};
};
}  // namespace sparks
