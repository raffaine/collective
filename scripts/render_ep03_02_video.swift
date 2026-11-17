import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

// 35 unique eye-level walking scenes across 1h 11m 45s (~2m 03s per vista, zero repetition)
let scenes: [SceneSpec] = [
    // Part I: Layer 5A — Elinor Ostrom's 8 Commons Principles, VSM System 5 & Human Scales (00:00 - 14:21)
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_03.png", actLabel: "PART I // LAYER 5A: OSTROM'S COMMONS PRINCIPLES", placeLabel: "Circular Timber Council Pavilion & Meadow"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_01.png", actLabel: "PART I // LAYER 5A: SCALE 1 (N=1 SOLO STEWARD)", placeLabel: "Solo Homesteader's Dusk Garage Workshop"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_02.png", actLabel: "PART I // LAYER 5A: SCALE 2 (Ns>=1, Np>1 POD)", placeLabel: "Neighborhood Tool-Crib & Tiffin Porch"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_04.png", actLabel: "PART I // LAYER 5A: ECOLOGICAL CONGRUENCE (F_ECO)", placeLabel: "Old-Growth Mother-Tree Sanctuary Boardwalk"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_11.png", actLabel: "PART I // LAYER 5A: VSM SYSTEM 5 ALGEDONIC LOOP", placeLabel: "Wooden-and-Brass Bedroom Sanctuary Latch"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_05.png", actLabel: "PART I // LAYER 5A: NESTED POLYCENTRIC SWARMS", placeLabel: "Timber-Frame Barn-Raising Courtyard"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_01.png", actLabel: "PART I // LAYER 5A: CELLULAR SUBSIDIARITY", placeLabel: "Cedar Utility Porch & Red Disconnect Lever"),

    // Part II: Layer 5B — Kinship Pluralism (Omicron), Trust Rings R0–R6 & Sybil EigenTrust (14:21 - 28:42)
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_07.png", actLabel: "PART II // LAYER 5B: CHOSEN KINSHIP COVENANTS", placeLabel: "Welcoming Hearth Parlor & Five Stoneware Cups"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_08.png", actLabel: "PART II // LAYER 5B: <500MS SAFE-HARBOR EXIT", placeLabel: "Sanctuary Lodge & Unilateral Brass Room Key"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_07.png", actLabel: "PART II // LAYER 5B: TRUST RINGS R0 TO R2", placeLabel: "Unfenced Backyard Garden Commons"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_09.png", actLabel: "PART II // LAYER 5B: EMERITUS ELDER SAGEHOOD", placeLabel: "Zero-Step Elder Courtyard & Lavender Path"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_06.png", actLabel: "PART II // LAYER 5B: PHYSICAL NFC VOUCH ROOTS", placeLabel: "Zero-Step Cedar Ramp & Sensory-Calm Slate"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_05.png", actLabel: "PART II // LAYER 5B: SYBIL MAX-FLOW CUT BOUNDS", placeLabel: "Canopy Boardwalk Over Living Watershed"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_07.png", actLabel: "PART II // LAYER 5B: 24-BYTE BLINDED CLAIMHANDLE", placeLabel: "Sunlit Woodshop & Unlinkable BBS+ Interlock"),

    // Part III: Layer 6A — Ontological Surface, Indigenous Named Graphs & Stoichiometric Cycles (28:42 - 43:03)
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_13.png", actLabel: "PART III // LAYER 6A: INDIGENOUS NAMED GRAPHS", placeLabel: "Agroforestry Chestnut, Oak & Hazel Grove"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_11.png", actLabel: "PART III // LAYER 6A: KINCENTRIC WATERSHED TEK", placeLabel: "Valley Greenway & Salmon Creek Trail"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_04.png", actLabel: "PART III // LAYER 6A: RDFC-1.0 & LENGTH-PREFIXED CID", placeLabel: "Timber Studio & Ontological Graph Workbench"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_06.png", actLabel: "PART III // LAYER 6A: STOICHIOMETRIC NEED DAGS", placeLabel: "Communal Kitchen & Pea-Soup Stockpot Slate"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_13.png", actLabel: "PART III // LAYER 6A: CLOSED BYPRODUCT LOOPS", placeLabel: "Dawn Thermophilic Compost & Biochar Shed"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_06.png", actLabel: "PART III // LAYER 6A: DECLARATIVE CULTIVAR GRAPHS", placeLabel: "Heirloom Fabaceae & Permaculture Street"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_02.png", actLabel: "PART III // LAYER 6A: MULTI-LAYER BLUEPRINTS", placeLabel: "Hydronic Server Heat-Recovery to Greenhouse"),

    // Part IV: Layer 6B — Bioregional Agora, Dissenter Co-Signature & Pure-Integer Clearing (43:03 - 57:24)
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_12.png", actLabel: "PART IV // LAYER 6B: THE BIOREGIONAL AGORA", placeLabel: "Car-Free Urban Neighborhood Plaza"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_10.png", actLabel: "PART IV // LAYER 6B: DISSENTER CO-SIGNATURE & PEACE", placeLabel: "Unmediated Parlor & Brass Mic Kill-Switch"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_07.png", actLabel: "PART IV // LAYER 6B: DELAY-TOLERANT AGORA CRDT", placeLabel: "Valley Greenway & Cargo-Trike Courier Deck"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_10.png", actLabel: "PART IV // LAYER 6B: PURE-INTEGER EXERGY AGORA", placeLabel: "Riverside Stone Quay & Electric Cargo Boat"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_08.png", actLabel: "PART IV // LAYER 6B: CARE NON-COMMENSURABILITY", placeLabel: "Clinic Vestibule & Red Trauma Pull-Cord"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_09.png", actLabel: "PART IV // LAYER 6B: DIGITAL SILENCE IN R0", placeLabel: "Screenless Mass-Timber Living Room & Hearth"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_14.png", actLabel: "PART IV // LAYER 6B: SEASONAL COMMONS JUBILEE", placeLabel: "Twilight Courtyard & Lantern Path"),

    // Part V: Layer 7 — Healthy Circular Extraction, Catered GPO Sourcing & Dual-Membrane Carapace (57:24 - 1:11:45)
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_12.png", actLabel: "PART V // LAYER 7: HEALTHY CIRCULAR EXTRACTION", placeLabel: "Urban Unbuilding Salvage & Pocket Park"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_08.png", actLabel: "PART V // LAYER 7: PERMANENT ATOM INTERNALIZATION", placeLabel: "Solar Foundry Courtyard & Recycled Bronze"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_11.png", actLabel: "PART V // LAYER 7: CATERED ETHICAL GPO SOURCING", placeLabel: "Third-Place Cafe & Cooperative Carapace"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_11.png", actLabel: "PART V // LAYER 7: EXERGY SHIELD & ROBIN HOOD SIPHON", placeLabel: "Golden-Hour Urban Cooperative Market Lane"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_13.png", actLabel: "PART V // LAYER 7: DUAL-CHASSIS CLT PARTITION LOCK", placeLabel: "Community Land Trust & Conservation Hedgerow"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_14.png", actLabel: "PART V // SCENARIO OMEGA: CARAPACE SUNSET", placeLabel: "Switchgear Pavilion & Open Utility Breakers"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_15.png", actLabel: "PART V // SCENARIO OMEGA: PLANETARY LIGHTHOUSE", placeLabel: "High Ridge Planetary Lighthouse & Valley")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 5.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 720, height: 86)
    ctx.setFillColor(CGColor(red: 0.07, green: 0.08, blue: 0.10, alpha: 0.62))
    let path = CGPath(roundedRect: boxRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.addPath(path)
    ctx.fillPath()

    ctx.setFillColor(CGColor(red: 0.95, green: 0.77, blue: 0.19, alpha: 0.90))
    ctx.fill(CGRect(x: 48, y: 42, width: 4, height: 86))

    let graphicsCtx = NSGraphicsContext(cgContext: ctx, flipped: false)
    NSGraphicsContext.saveGraphicsState()
    NSGraphicsContext.current = graphicsCtx

    let topAttrs: [NSAttributedString.Key: Any] = [
        .font: NSFont.monospacedSystemFont(ofSize: 13, weight: .bold),
        .foregroundColor: NSColor(calibratedRed: 0.95, green: 0.77, blue: 0.19, alpha: 0.92)
    ]
    let midAttrs: [NSAttributedString.Key: Any] = [
        .font: NSFont.systemFont(ofSize: 18, weight: .semibold),
        .foregroundColor: NSColor(calibratedWhite: 0.96, alpha: 0.95)
    ]
    let botAttrs: [NSAttributedString.Key: Any] = [
        .font: NSFont.monospacedSystemFont(ofSize: 12, weight: .regular),
        .foregroundColor: NSColor(calibratedWhite: 0.78, alpha: 0.85)
    ]

    NSString(string: "RADIO LAMBDA // EP. 03 (DIRECTOR'S CUT): LAYERS 5, 6 & 7").draw(at: NSPoint(x: 66, y: 99), withAttributes: topAttrs)
    NSString(string: placeLabel).draw(at: NSPoint(x: 66, y: 71), withAttributes: midAttrs)
    NSString(string: actLabel).draw(at: NSPoint(x: 66, y: 50), withAttributes: botAttrs)

    NSGraphicsContext.restoreGraphicsState()
    return ctx.makeImage()!
}

func drawWalkingScene(into ctx: CGContext, img: CGImage, progress: Double, index: Int, alpha: CGFloat = 1.0) {
    let smooth = progress * progress * (3.0 - 2.0 * progress)
    let zoom = CGFloat(1.0 + 0.12 * smooth)
    let panDir: CGFloat = (index % 2 == 0) ? 1.0 : -1.0
    let panX = panDir * CGFloat(18.0 * (smooth - 0.5))
    let panY = CGFloat(-8.0 * (smooth - 0.5))

    let drawW = CGFloat(width) * zoom
    let drawH = CGFloat(height) * zoom
    let dx = (CGFloat(width) - drawW) * 0.5 + panX
    let dy = (CGFloat(height) - drawH) * 0.48 + panY

    if alpha < 1.0 {
        ctx.saveGState()
        ctx.setAlpha(alpha)
        ctx.draw(img, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
        ctx.restoreGState()
    } else {
        ctx.draw(img, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    }
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    let audioPath = "doc/podcast/episodes/03.02.episode.m4a"
    let tempVideoPath = "/tmp/ep03_02_silent_video.mp4"
    let finalOutputPath = "doc/podcast/episodes/03.02.episode_youtube.mp4"

    try? FileManager.default.removeItem(atPath: tempVideoPath)
    try? FileManager.default.removeItem(atPath: finalOutputPath)

    let audioAsset = AVURLAsset(url: URL(fileURLWithPath: audioPath))
    let durationCM = try await audioAsset.load(.duration)
    let totalDurationSec = CMTimeGetSeconds(durationCM)
    print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", totalDurationSec, totalDurationSec / 60.0))

    let baseImages = scenes.map { loadNormalizedCGImage($0.path) }
    let hudImages = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }

    let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
    let videoSettings: [String: Any] = [
        AVVideoCodecKey: AVVideoCodecType.h264,
        AVVideoWidthKey: width,
        AVVideoHeightKey: height,
        AVVideoCompressionPropertiesKey: [
            AVVideoAverageBitRateKey: 1_800_000,
            AVVideoMaxKeyFrameIntervalKey: Int(fps) * 3
        ]
    ]
    let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
    writerInput.expectsMediaDataInRealTime = false

    let pixelBufferAttrs: [String: Any] = [
        kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
        kCVPixelBufferWidthKey as String: width,
        kCVPixelBufferHeightKey as String: height
    ]
    let adaptor = AVAssetWriterInputPixelBufferAdaptor(
        assetWriterInput: writerInput,
        sourcePixelBufferAttributes: pixelBufferAttrs
    )

    writer.add(writerInput)
    writer.startWriting()
    writer.startSession(atSourceTime: .zero)

    let totalFrames = Int(ceil(totalDurationSec * Double(fps)))
    let segDuration = totalDurationSec / Double(scenes.count)

    print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

    for frameIdx in 0..<totalFrames {
        while !writerInput.isReadyForMoreMediaData {
            usleep(2000)
        }
        autoreleasepool {
            let t = Double(frameIdx) / Double(fps)
            let rawIdx = Int(t / segDuration)
            let idx = min(max(rawIdx, 0), scenes.count - 1)
            let segStart = Double(idx) * segDuration
            let localT = t - segStart
            let progress = min(max(localT / segDuration, 0.0), 1.0)

            var pb: CVPixelBuffer?
            CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pb)
            if let pixelBuffer = pb {
                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                if let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer),
                   let ctx = CGContext(
                       data: baseAddr,
                       width: width,
                       height: height,
                       bitsPerComponent: 8,
                       bytesPerRow: CVPixelBufferGetBytesPerRow(pixelBuffer),
                       space: colorSpace,
                       bitmapInfo: bitmapInfo
                   ) {
                    ctx.interpolationQuality = .medium
                    drawWalkingScene(into: ctx, img: baseImages[idx], progress: progress, index: idx)

                    var activeHud = hudImages[idx]
                    if idx < scenes.count - 1 && (segDuration - localT) < crossfadeSec {
                        let fadeAlpha = CGFloat(1.0 - (segDuration - localT) / crossfadeSec)
                        let nextProgress = max(0.0, (localT - (segDuration - crossfadeSec)) / segDuration)
                        drawWalkingScene(into: ctx, img: baseImages[idx + 1], progress: nextProgress, index: idx + 1, alpha: fadeAlpha)
                        if fadeAlpha > 0.5 {
                            activeHud = hudImages[idx + 1]
                        }
                    }
                    ctx.draw(activeHud, in: CGRect(x: 0, y: 0, width: width, height: height))
                }
                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let frameTime = CMTime(value: Int64(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: frameTime)
            }
        }

        if frameIdx % (Int(fps) * 300) == 0 && frameIdx > 0 {
            let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
            print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
        }
    }

    writerInput.markAsFinished()
    await writer.finishWriting()
    print("Silent video track rendered. Now muxing audio losslessly...")

    let mixComp = AVMutableComposition()
    let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

    guard let compVideoTrack = mixComp.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid),
          let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first else {
        fatalError("Failed to load video track for muxing")
    }
    try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

    guard let compAudioTrack = mixComp.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid),
          let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first else {
        fatalError("Failed to load audio track for muxing")
    }
    try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

    guard let exporter = AVAssetExportSession(asset: mixComp, presetName: AVAssetExportPresetPassthrough) else {
        fatalError("Failed to create AVAssetExportSession")
    }
    try await exporter.export(to: URL(fileURLWithPath: finalOutputPath), as: .mp4)
    try? FileManager.default.removeItem(atPath: tempVideoPath)
    print("SUCCESS: Created YouTube video at \(finalOutputPath)")
    semaphore.signal()
}

semaphore.wait()
